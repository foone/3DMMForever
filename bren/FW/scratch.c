/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: scratch.c 1.7 1995/08/31 16:29:46 sam Exp $
 * $Locker:  $
 *
 * Manage a temporary scratchpad for the rest of the system
 *
 * Will only allow 1 buffer to be allocated at any time
 */
#include "fw.h"
#include "brassert.h"

/*
 * Allocate a scratch buffer of the given size
 */
void *BR_PUBLIC_ENTRY BrScratchAllocate(br_size_t size)
{
    if (fw.scratch_inuse)
        BR_ERROR0("Scratchpad not available");

    fw.scratch_last = size;

    if (size > fw.scratch_size)
    {
        if (fw.scratch_ptr)
            BrResFree(fw.scratch_ptr);

        fw.scratch_ptr = BrResAllocate(fw.res, size, BR_MEMORY_SCRATCH);
#if 0
		fw.scratch_size = BrResSize(fw.scratch_ptr);
#else
        fw.scratch_size = size;
#endif
    }

    fw.scratch_inuse = 1;

    return fw.scratch_ptr;
}

/*
 * Release a scratch buffer
 */
void BR_PUBLIC_ENTRY BrScratchFree(void *scratch)
{
    UASSERT(fw.scratch_ptr == scratch);

    fw.scratch_inuse = 0;

#if 0 /* Churn memory allocator */
	BrResFree(fw.scratch_ptr);
	fw.scratch_ptr = NULL;
	fw.scratch_size = 0;
#endif
}

/*
 * Free any allocated scratch buffer
 */
void BR_PUBLIC_ENTRY BrScratchFlush(void)
{
    if (fw.scratch_inuse)
        BR_ERROR0("Scratchpad cannot be flushed while in use");

    if (fw.scratch_ptr)
        BrResFree(fw.scratch_ptr);

    fw.scratch_ptr = NULL;
    fw.scratch_size = 0;
}

/*
 * Find out how much scratch space is 'easily' available
 */
br_size_t BR_PUBLIC_ENTRY BrScratchInquire(void)
{
    return fw.scratch_size;
}
