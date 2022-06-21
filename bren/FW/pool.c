/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: pool.c 1.13 1995/08/31 16:29:42 sam Exp $
 * $Locker:  $
 *
 * Memory pool allocation (fixed size objects)
 */
#include "fw.h"
#include "brassert.h"

static char rscid[] = "$Id: pool.c 1.13 1995/08/31 16:29:42 sam Exp $";

#if BR_POOL_DEBUG
#undef PoolAllocate
#undef PoolFree
#endif

br_pool *BR_PUBLIC_ENTRY BrPoolAllocate(int block_size, int chunk_size, br_uint_8 mem_type)
{
    br_pool *pool = BrResAllocate(NULL, sizeof(*pool), BR_MEMORY_POOL);

    pool->free = NULL;

    if (block_size < sizeof(br_pool_block))
        block_size = sizeof(br_pool_block);

    pool->block_size = (block_size + BR_POOL_ALIGN) & ~BR_POOL_ALIGN;
    pool->chunk_size = chunk_size;
    pool->mem_type = mem_type;
#if BR_POOL_DEBUG
    pool->count = 0;
    pool->max_count = 0;
#endif

    return pool;
}

void BR_PUBLIC_ENTRY BrPoolFree(br_pool *pool)
{
    UASSERT(BrResCheck(pool, 1));
    UASSERT(BrResClass(pool) == BR_MEMORY_POOL);
    UASSERT(pool != NULL);

    BrResFree(pool);
}

/*
 * Goes through the child resources of the pool,
 * and for each one that has a class matching the
 * pool's mem_type, divides it up into blocks and
 * adds them to the free list
 */

static int BR_CALLBACK PoolBlockRelink(char *chunk, br_pool *pool)
{
    int size;
    br_pool_block *free_ptr;

    /*
     * Ignore blocks that are not of the appropriate type
     */
    if (BrResClass(chunk) != (br_uint_8)pool->mem_type)
        return 0;

    /*
     * Go through block linking it into free list
     */
    free_ptr = pool->free;

    for (size = BrResSize(chunk); size >= pool->block_size; size -= pool->block_size)
    {

        ((br_pool_block *)chunk)->next = free_ptr;
        free_ptr = (br_pool_block *)chunk;
        chunk += pool->block_size;
    }

    pool->free = free_ptr;

    return 0;
}

/*
 * Add another chunk of blocks to the end of the
 * pool
 */
void BR_ASM_CALLBACK BrPoolAddChunk(br_pool *pool)
{
    br_pool_block *bp, *obp;
    unsigned int i;
    char *ptr;

    /*
     * Allocate a new block of the appropriate size
     */
    bp = BrResAllocate(pool, pool->block_size * pool->chunk_size, (br_uint_8)pool->mem_type);

    /*
     * Link the block in chunk into free list
     */
    PoolBlockRelink((char *)bp, pool);

#if 0
	obp = pool->free;

	for(i=0; i<pool->chunk_size; i++) {
		bp->next = obp;
		obp = bp;
		bp = (br_pool_block *)((char *)bp + pool->block_size);
	}

	pool->free = obp;
#endif
}

void *BR_PUBLIC_ENTRY BrPoolBlockAllocate(br_pool *pool)
{
    br_pool_block *bp;

    if (pool->free == NULL)
        BrPoolAddChunk(pool);

#if BR_POOL_DEBUG
    if (++pool->count > pool->max_count)
        pool->max_count = pool->count;
#endif

    bp = pool->free;
    pool->free = bp->next;
    return bp;
}

void BR_PUBLIC_ENTRY BrPoolBlockFree(br_pool *pool, void *b)
{
    br_pool_block *bp = (br_pool_block *)b;

    bp->next = pool->free;
    pool->free = bp;

#if BR_POOL_DEBUG
    pool->count--;
#endif
}

void BR_PUBLIC_ENTRY BrPoolEmpty(br_pool *pool)
{
    /*
     * Clear out free list
     */
    pool->free = NULL;

    /*
     * Go through each child resource and add
     * appropriate chunks to the free list
     */
    BrResChildEnum(pool, (br_resenum_cbfn *)PoolBlockRelink, pool);

#if BR_POOL_DEBUG
    pool->count = 0;
    pool->max_count = 0;
#endif
}
