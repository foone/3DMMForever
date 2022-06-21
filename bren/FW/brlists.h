/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: brlists.h 1.4 1995/02/22 21:41:25 sam Exp $
 * $Locker:  $
 *
 * Doubly linked list supoprt (Based on Amiga EXEC!)
 */
#ifndef _BRLISTS_H_
#define _BRLISTS_H_

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     **	Full featued doubly linked lists
     **/

    /*
     * Anchor structure at front of list entries
     */
    typedef struct br_node
    {
        struct br_node *next;
        struct br_node *prev;
    } br_node;

    /*
     * Anchor structure for whole list - looks like two nodes overlapping.
     * The _null memeber is always NULL, providing the termination at
     * both ends of the list
     */
    typedef struct br_list
    {
        br_node *head;
        br_node *_null;
        br_node *tail;
    } br_list;

#define BR_HEAD(l) (void *)(((struct br_list *)(l))->head)
#define BR_TAIL(l) (void *)(((struct br_list *)(l))->tail)

#define BR_NEXT(n) (void *)(((struct br_node *)(n))->next)
#define BR_PREV(n) (void *)(((struct br_node *)(n))->prev)
#define BR_INSERTED(n) (void *)(((struct br_node *)(n))->next)

/*
 * Walk a pointer forwards along a list
 *
 * eg:
 *	BR_FOR_LIST(sprites,sptr) {
 *		sptr->x = 10;
 *	}
 */
#define BR_FOR_LIST(list, ptr) for ((ptr) = BR_HEAD(list); BR_NEXT(ptr); ptr = BR_NEXT(ptr))

/*
 * Walk a pointer backwards along a list
 *
 * eg:
 *	BR_FOR_LIST_R(sprites,sptr) {
 *		sptr->x = 10;
 *	}
 */
#define BR_FOR_LIST_R(list, ptr) for ((ptr) = BR_TAIL(list); BR_PREV(ptr); ptr = BR_PREV(ptr))

/*
 * Macros around exported functions that hide type casting
 */
#define BR_ADDHEAD(l, n) BrAddHead((struct br_list *)(l), (struct br_node *)(n))
#define BR_ADDTAIL(l, n) BrAddTail((struct br_list *)(l), (struct br_node *)(n))
#define BR_REMHEAD(l) ((void *)BrRemHead((struct br_list *)(l)))
#define BR_REMTAIL(l) ((void *)BrRemTail((struct br_list *)(l)))
#define BR_INSERT(l, n1, n2) BrInsert((struct br_list *)(l), (struct br_node *)(n1), (struct br_node *)(n2))
#define BR_REMOVE(n) ((void *)BrRemove((struct br_node *)(n)))

    /**
     **	Simple lists that only use a single head pointer
     **/
    typedef struct br_simple_node
    {
        struct br_simple_node *next;
        struct br_simple_node **prev;
    } br_simple_node;

    /*
     * Anchor structure for whole list - just a single head pointer
     */
    typedef struct br_simple_list
    {
        struct br_simple_node *head;
    } br_simple_list;

#define BR_SIMPLEHEAD(l) (void *)(((br_simple_list *)(l))->head)
#define BR_SIMPLENEXT(n) (void *)(((br_simple_node *)(n))->next)

#define BR_SIMPLEINSERTED(n) (void *)(((br_simple_node *)(n))->prev)

#define BR_FOR_SIMPLELIST(list, ptr) for ((ptr) = BR_SIMPLEHEAD(list); (ptr); (ptr) = BR_SIMPLENEXT(ptr))

/*
 * Macros around exported functions that hide type casting
 */
#define BR_SIMPLEADDHEAD(l, n) BrSimpleAddHead((br_simple_list *)(l), (br_simple_node *)(n))
#define BR_SIMPLEREMHEAD(l) ((void *)BrSimpleRemHead((br_simple_list *)(l)))
#define BR_SIMPLEINSERT(l, n1, n2) BrSimpleInsert((br_simple_list *)(l), (br_simple_node *)(n1), (br_simple_node *)(n2))
#define BR_SIMPLEREMOVE(n) ((void *)BrSimpleRemove((br_simple_node *)(n)))

/*
 * Protoypes in lists.c
 */
#ifndef _NO_PROTOTYPES
    void BR_PUBLIC_ENTRY BrNewList(br_list *list);
    void BR_PUBLIC_ENTRY BrAddHead(br_list *list, br_node *node);
    void BR_PUBLIC_ENTRY BrAddTail(br_list *list, br_node *node);

    br_node *BR_PUBLIC_ENTRY BrRemHead(br_list *list);
    br_node *BR_PUBLIC_ENTRY BrRemTail(br_list *list);

    void BR_PUBLIC_ENTRY BrInsert(br_list *list, br_node *here, br_node *node);
    br_node *BR_PUBLIC_ENTRY BrRemove(br_node *node);

    void BR_PUBLIC_ENTRY BrSimpleNewList(br_simple_list *list);
    void BR_PUBLIC_ENTRY BrSimpleAddHead(br_simple_list *list, br_simple_node *node);
    br_simple_node *BR_PUBLIC_ENTRY BrSimpleRemHead(br_simple_list *list);
    void BR_PUBLIC_ENTRY BrSimpleInsert(br_simple_list *list, br_simple_node *here, br_simple_node *node);
    br_simple_node *BR_PUBLIC_ENTRY BrSimpleRemove(br_simple_node *node);

#endif

#ifdef __cplusplus
};
#endif
#endif
