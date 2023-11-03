/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: register.c 1.12 1995/03/16 11:57:07 sam Exp $
 * $Locker:  $
 *
 * Generic code for lists of registered items. Used for models, materials,
 * textures, tables, cameras, lights, scenes
 *
 * Each registry is a doubly linked list of chunks of data, the first
 * member of an item should be a pointer the the item's name
 */
#include "fw.h"
#include "brassert.h"

static char rscid[] = "$Id: register.c 1.12 1995/03/16 11:57:07 sam Exp $";

/*
 * Compare a pattern against a string
 *
 * Return true if pattern matches string
 *
 * Patterns have the magic characters '*' and '?'
 *
 * '*' matches 0 or more characters
 * '?' matches any single character
 * '/' terminates the pattern
 *
 * Patterns are anchored at start and end of string
 *
 * Recursive approach, implemented with tail recursion -
 *
 * match (p,s)
 *
 *	case first(p) in
 *
 *	NULL	- TRUE if s is empty else FALSE
 *
 *	'*'		- TRUE if match rest(q) against any substring from s to end of s
 *				else
 *			  FALSE
 *
 *	'?'		- TRUE if first(s) != NULL and match(rest(p),rest(r))
 *
 *	default - TRUE if first(p) == first(s) and match(rest(p),rest(r))
 *
 */

#if 1 /* case insensitive for the moment */
#define UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - ('a' - 'A')) : (c))
#define MATCH_CHAR(a, b) (UPPER(a) == UPPER(b))
#else
#define MATCH_CHAR(a, b) ((a) == (b))
#endif

int NamePatternMatch(char *p, char *s)
{
    char *cp;

    /*
     * A NULL pattern matches everything
     */
    if (p == NULL)
        return 1;

    /*
     * A NULL string never matches
     */
    if (s == NULL)
        return 0;

    for (;;)
        switch (*p)
        {

        case '/':
        case '\0':
            /*
             * Empty pattern only matches empty string
             */
            return *s == '\0';

        case '*':
            /*
             * Match p+1 in any position from s to end of s
             */
            cp = s;
            do
                if (NamePatternMatch(p + 1, cp))
                    return 1;
            while (*cp++);

            return 0;

        case '?':
            /*
             * Match any character followed by matching(p+1, s+1)
             */
            if (*s == '\0')
                return 0;

            p++, s++; /* Tail recurse */
            continue;

        default:
            /*
             * Match this character followed by matching(p+1, s+1)
             */
            if (!MATCH_CHAR(*s, *p))
                return 0;

            p++, s++; /* Tail recurse */
            continue;
        }
}

/*
 * Initialise a registry
 */
void *RegistryNew(br_registry *reg)
{
    UASSERT(reg != NULL);

    /*
     * Intitialise linked list
     */
    BrNewList(&reg->list);

    /*
     * List is empty
     */
    reg->count = 0;

    return reg;
}

/*
 * Release all items on a registry
 */
void *RegistryClear(br_registry *reg)
{
    br_registry_entry *e;

    UASSERT(reg != NULL);

    while (e = BR_HEAD(&reg->list), BR_NEXT(e))
    {
        BR_REMOVE(e);
        BrResFree(e);
    }

    /*
     * List is empty
     */
    reg->count = 0;

    return reg;
}

/*
 * Add one new item to a registry
 */
void *RegistryAdd(br_registry *reg, void *item)
{
    br_registry_entry *e;

    UASSERT(reg != NULL);
    UASSERT(item != NULL);

    e = BrResAllocate(fw.res, sizeof(*e), BR_MEMORY_REGISTRY);
    e->item = item;
    BR_ADDHEAD(reg, e);

    reg->count++;

    return item;
}

/*
 * Add items from a table of pointers to a registry
 */
int RegistryAddMany(br_registry *reg, void **items, int n)
{
    int i;
    UASSERT(reg != NULL);
    UASSERT(items != NULL);

    /*
     * Walk through table adding items
     */
    for (i = 0; i < n; i++)
        RegistryAdd(reg, *items++);

    return n;
}

/*
 * Remove a single item from the registry
 */
void *RegistryRemove(br_registry *reg, void *item)
{
    br_registry_entry *e;
    void *r;

    UASSERT(reg != NULL);
    UASSERT(item != NULL);

    /*
     * Find item in list
     */
    BR_FOR_LIST(&reg->list, e)
    if (e->item == item)
        break;

    /*
     * If item was not in list, return NULL
     */
    if (!BR_NEXT(e))
        return NULL;

    /*
     * Take item of list, remember contents, and free node
     */
    BR_REMOVE(e);
    r = e->item;
    BrResFree(e);

    reg->count--;

    return r;
}

/*
 * Remove a table of referenced items from a registry
 */
int RegistryRemoveMany(br_registry *reg, void **items, int n)
{
    int i, r;

    UASSERT(reg != NULL);
    UASSERT(items != NULL);

    /*
     * Remove all the items from a table, keeping a count of how
     * many were actually removed
     */
    for (i = 0, r = 0; i < n; i++)
        if (RegistryRemove(reg, *items++))
            r++;

    return r;
}

/*
 * Find the first item in registry whose name matches a given pattern
 *
 * If no item can be found, then call a find_failed hook if it exists
 */
void *RegistryFind(br_registry *reg, char *pattern)
{
    br_registry_entry *e;

    UASSERT(reg != NULL);

    /*
     * Find item in list
     */
    BR_FOR_LIST(&reg->list, e)
    if (NamePatternMatch(pattern, *e->item))
        return e->item;

    if (reg->find_failed_hook)
        return reg->find_failed_hook(pattern);
    else
        return NULL;
}

int RegistryFindMany(br_registry *reg, char *pattern, void **items, int max)
{
    br_registry_entry *e;
    int n = 0;

    /*
     * Find all matching items in list
     */
    BR_FOR_LIST(&reg->list, e)
    {
        /*
         * make sure there is space in output table
         */
        if (n >= max)
            break;

        /*
         * If entry matches, add to table
         */
        if (NamePatternMatch(pattern, *e->item))
        {
            *items++ = e->item;
            n++;
        }
    }

    return n;
}

/*
 * Count how many items in registry match pattern
 *
 * If pattern == NULL, return total in registry
 */
int RegistryCount(br_registry *reg, char *pattern)
{
    br_registry_entry *e;
    int n;

    UASSERT(reg != NULL);

    if (pattern == NULL)
        return reg->count;

    /*
     * Find all matching items in list
     */
    n = 0;

    BR_FOR_LIST(&reg->list, e)
    if (NamePatternMatch(pattern, *e->item))
        n++;

    return n;
}

/*
 * Call a function for every item in a registry. Stop early if callback
 * returns !=0, and return that value
 */
int RegistryEnum(br_registry *reg, char *pattern, br_enum_cbfn *callback, void *arg)
{
    br_registry_entry *e;
    int r;

    UASSERT(reg != NULL);
    UASSERT(callback != NULL);

    /*
     * If pattern in NULL, invoke callback for _EVERYTHING_
     * else invoke callback for items that match pattern
     */
    if (pattern == NULL)
    {
        BR_FOR_LIST_R(&reg->list, e)
        if (r = callback(e->item, arg))
            return r;
    }
    else
    {
        BR_FOR_LIST_R(&reg->list, e)
        if (NamePatternMatch(pattern, *e->item))
            if (r = callback(e->item, arg))
                return r;
    }
    return 0;
}
