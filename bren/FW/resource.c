/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: resource.c 1.9 1995/08/31 16:29:45 sam Exp $
 * $Locker:  $
 *
 * Generic code for maintaining a hierachy of resources
 *
 * Each resource has -
 *		A class (one of the BR_RES_xxx constants)
 *		A size
 *		A block of memory of 'size' bytes
 *		0 or more child resources
 *
 * XXX
 *	Add optional source file/line tracking
 */
#include <stddef.h>
#include <string.h>

#if DEBUG
#include <stdio.h>
#endif

#include "fw.h"
#include "brassert.h"

static char rscid[] = "$Id: resource.c 1.9 1995/08/31 16:29:45 sam Exp $";

/*
 * The granularity of resource sizes
 */
#define BR_RES_GRANULARITY 8
#define BR_RES_MAGIC 0xDEADBEEF

/*
 * The control structure prepended onto resource blocks
 */
struct resource_header
{
    br_simple_node node;
    br_simple_list children;
    br_uint_8 class;     /* Class of resource 			 	*/
    br_int_32 size : 32; /* Size of resource in 				*/
                         /*  BR_RES_GRANULARITY units 		*/
#if BR_RES_TAGGING
    br_uint_32 magic_num;
    void *magic_ptr;
#endif
};

#if BR_RES_TAGGING
#define ISRESOURCE(res) ((res->magic_ptr == res) && (res->magic_num == BR_RES_MAGIC))
#else
#define ISRESOURCE(res) 1
#endif

#define RES_BOUNDARY 16
#define RES_ALIGN(x) ((void *)(((br_uint_ptr)(x) + (RES_BOUNDARY - 1)) & ~(RES_BOUNDARY - 1)))

/*
 * Align the resource pointer
 */
static void *ResToUser(struct resource_header *r)
{
    /*
     * Move over the resource_header
     */
    r++;

    /*
     * Allocations should always be at least long word aligned
     */
    ASSERT((((br_uint_ptr)r) & 3) == 0);

    /*
     * Bump pointer up to next boundary
     */
    return RES_ALIGN(r);
}

/*
 * Unalign the resource pointer
 */
static struct resource_header *UserToRes(void *r)
{
    union {
        br_uint_32 *p32;
        br_uint_8 *p8;
        struct resource_header *pres;
    } l = {r};

#if 0
	ASSERT( ((int) r & (RES_BOUNDARY-1)) == 0);
#endif

    /*
     * the last long-word of the resource will always be non-zero -
     * class/size or magic_num
     *
     * Move pointer back until we hit it
     */

    while (*l.p32 != BR_RES_MAGIC)
    {
        l.p32--;
    }
    l.p8 -= offsetof(struct resource_header, magic_num);

    return l.pres;
}

/*
 * Create a new resource block of the given class, with 'size' bytes associated
 * with it.
 *
 * If parent is not NULL it adds the new resource as a child
 *
 * Returns a pointer to the first byte of the resource data
 */
void *BR_PUBLIC_ENTRY BrResAllocate(void *vparent, br_size_t size, int class)
{
    struct resource_header *res;
    struct resource_header *parent;

    UASSERT(fw.resource_class_index[class] != NULL);

    /*
     * Work out size in BR_RES_GRANULARITY units
     */
    size = (size + sizeof(struct resource_header) + (BR_RES_GRANULARITY - 1)) / BR_RES_GRANULARITY;

    res = BrMemAllocate(size * BR_RES_GRANULARITY + (RES_BOUNDARY - 1), class);

    res->class = class;
    res->size = size;
    BrSimpleNewList(&res->children);

#if BR_RES_TAGGING
    res->magic_num = BR_RES_MAGIC;
    res->magic_ptr = res;
#endif

    if (vparent)
    {
        parent = UserToRes(vparent);
        BR_SIMPLEADDHEAD(&parent->children, res);
    }

    /*
     * Return pointer to used data
     */
    return ResToUser(res);
}

/*
 * Release a resource block - first calls BrResFree() for any child resources
 *
 * Removes resource from any parent list
 *
 * If the resource class has a destructor, that function is called
 */
static void BrResInternalFree(struct resource_header *res)
{
    UASSERT(ISRESOURCE(res));
    UASSERT(fw.resource_class_index[res->class] != NULL);

    /*
     * Free any children
     */
    while (BR_SIMPLEHEAD(&res->children))
    {
        BrResInternalFree((struct resource_header *)BR_SIMPLEREMOVE(BR_SIMPLEHEAD(&res->children)));
    }

    /*
     * Remove from any parent list
     */
    if (BR_SIMPLEINSERTED(res))
    {
        BR_SIMPLEREMOVE(res);
    }

    /*
     * Call class destructor
     */
    br_resource_class *res_class = fw.resource_class_index[res->class];
    if (res_class && res_class->free_cb)
    {
        res_class->free_cb(ResToUser(res), res->class, res->size);
    }
#if BR_RES_TAGGING
    /*
     * Make sure memeory is no longer tagged as a resource
     */
    res->magic_num = 1;
    res->magic_ptr = NULL;
#endif

    /*
     * Release block
     */
    BrMemFree(res);
}

/*
 * Public entry for ResFree
 */
void BR_PUBLIC_ENTRY BrResFree(void *vres)
{
    UASSERT(vres != NULL);

    BrResInternalFree(UserToRes(vres));
}

/*
 * Add a resource as a child of another
 */
void *BR_PUBLIC_ENTRY BrResAdd(void *vparent, void *vres)
{
    struct resource_header *res = UserToRes(vres);
    struct resource_header *parent = UserToRes(vparent);

    UASSERT(vres != NULL);
    UASSERT(vparent != NULL);

    UASSERT(ISRESOURCE(res));
    UASSERT(ISRESOURCE(parent));

    /*
     * Remove from any parent list
     */
    if (BR_SIMPLEINSERTED(res))
        BR_SIMPLEREMOVE(res);

    BR_SIMPLEADDHEAD(&parent->children, res);

    return vres;
}

/*
 * Remove resource from parent
 */
void *BR_PUBLIC_ENTRY BrResRemove(void *vres)
{
    struct resource_header *res = UserToRes(vres);

    UASSERT(vres != NULL);

    UASSERT(ISRESOURCE(res));

    BR_SIMPLEREMOVE(res);

    return vres;
}

/*
 * Return the class of a given resource
 */
br_uint_8 BR_PUBLIC_ENTRY BrResClass(void *vres)
{
    struct resource_header *res = UserToRes(vres);

    UASSERT(vres != NULL);

    UASSERT(ISRESOURCE(res));

    return res->class;
}

#if 0
/*
 * Return the parent of a given resource
 */
void * BR_PUBLIC_ENTRY BrResParent(void * vres)
{
	struct resource_header *res = UserToRes(vres);

	UASSERT(vres != NULL);

	UASSERT(ISRESOURCE(res));

	return  res->parent?ResToUser(res->parent):NULL;
}
#endif

/*
 * Return the size of a given resource
 */
br_uint_32 BR_PUBLIC_ENTRY BrResSize(void *vres)
{
    struct resource_header *res = UserToRes(vres);

    UASSERT(vres != NULL);
    UASSERT(ISRESOURCE(res));

    return (res->size * BR_RES_GRANULARITY) - sizeof(struct resource_header);
}

/*
 * Return the size of a resource, plus that of all it's children
 */

static br_uint_32 BR_CALLBACK ResSizeTotal(void *vres, br_uint_32 *ptotal)
{
    /*
     * Accumulate this size...
     */
    ptotal += BrResSize(vres);

    /*
     * ...then add the sizes of all the children
     */
    BrResChildEnum(vres, (br_resenum_cbfn *)ResSizeTotal, (void *)ptotal);

    return 0;
}

br_uint_32 BR_PUBLIC_ENTRY BrResSizeTotal(void *vres)
{
    br_uint_32 total = 0;

    ResSizeTotal(vres, &total);

    return total;
}

/*
 * Invoke a callback for each of the children of a resource
 */
br_uint_32 BR_PUBLIC_ENTRY BrResChildEnum(void *vres, br_resenum_cbfn *callback, void *arg)
{
    struct resource_header *res = UserToRes(vres);
    struct resource_header *rp;
    br_uint_32 r;

    UASSERT(vres != NULL);
    UASSERT(ISRESOURCE(res));
    ASSERT(callback != NULL);

    BR_FOR_SIMPLELIST(&res->children, rp)
    if (r = callback(ResToUser(rp), arg))
        return r;

    return 0;
}

/*
 * If tagging is enabled, then returns true if *vres is a resource
 * block, otherwise returns no_tag
 */
br_uint_32 BR_PUBLIC_ENTRY BrResCheck(void *vres, int no_tag)
{
#if BR_RES_TAGGING
    struct resource_header *res = UserToRes(vres);

    return (res->magic_ptr == res) && (res->magic_num == BR_RES_MAGIC);
#else
    return no_tag;
#endif
}

/*
 * strdup() equivalent
 */
char *BR_PUBLIC_ENTRY BrResStrDup(void *vparent, char *str)
{
    int l;
    char *nstr;

    UASSERT(vparent != NULL);
    UASSERT(str != NULL);

    l = strlen(str);

    nstr = BrResAllocate(vparent, l + 1, BR_MEMORY_STRING);

    strcpy(nstr, str);

    return nstr;
}

#if 0 /* DEBUG */
/*
 * Debug function that will dump a resource hierachy
 *
 * Invokes a callback with each text line of the dump
 */
static void InternalResourceDump(struct resource_header *res, void (*putline)(char *str, void *arg), void *arg, int level)
{
#if 1
	int i;
	char *cp = _br_scratch_string;
	struct resource_header *child;
	br_resource_class *rclass;

	/*
	 * Blank out first bit of line
	 */
	for(i=0; i< level*2; i++)
		*cp++ = ' ';

	/*
	 * Describe resource
	 */
 	rclass= fw.resource_class_index[(unsigned char )res->class];

	if(rclass == NULL) {
		sprintf(cp,"0x%08x ??? (%02X)", res+1, res->class);
		putline(_br_scratch_string,arg);
		return;
	}

	sprintf(cp,"0x%08x %-20s %d", ResToUser(res), rclass->identifier, res->size);
	putline(_br_scratch_string,arg);

	/*
	 * Display any children
	 */
	BR_FOR_SIMPLELIST(&res->children,child)
		InternalResourceDump(child, putline, arg, level+1);
#endif
}

void BR_PUBLIC_ENTRY BrResDump(void *vres, void (*putline)(char *str, void *arg), void *arg)
{
	struct resource_header *res = UserToRes(vres);

	InternalResourceDump(res, putline, arg, 0);
}
#endif
