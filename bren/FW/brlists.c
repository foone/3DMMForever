/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: brlists.c 1.4 1995/02/22 21:41:24 sam Exp $
 * $Locker:  $
 *
 * Doubly linked list support - has a full and simple version
 */

#include "fw.h"

static char rscid[] = "$Id: brlists.c 1.4 1995/02/22 21:41:24 sam Exp $";

/**
 ** Full features list handling
 **/

/*
 * Initialse a list structure to be empty list
 */
void BR_PUBLIC_ENTRY BrNewList(struct br_list *list)
{
    list->head = (struct br_node *)&(list->_null);
    list->tail = (struct br_node *)&(list->head);
    list->_null = NULL;
}

/*
 * Add node at head of list
 */
void BR_PUBLIC_ENTRY BrAddHead(struct br_list *list, struct br_node *node)
{
    node->prev = (struct br_node *)&(list->head);
    node->next = list->head;
    node->next->prev = node;
    list->head = node;
}

/*
 * Add node at tail of list
 */
void BR_PUBLIC_ENTRY BrAddTail(struct br_list *list, struct br_node *node)
{
    node->next = (struct br_node *)&(list->_null);
    node->prev = list->tail;
    node->prev->next = node;
    list->tail = node;
}

/*
 * Take node of head of list and return pointer to it
 */
struct br_node *BR_PUBLIC_ENTRY BrRemHead(struct br_list *list)
{
    struct br_node *n = list->head;

    /*
     * Trap case of empty list
     */
    if (n == (struct br_node *)&(list->_null))
        return NULL;

    list->head = n->next;

    return n;
}

/*
 * Take node of tail of list and return pointer to it
 */
struct br_node *BR_PUBLIC_ENTRY BrRemTail(struct br_list *list)
{
    struct br_node *n = list->tail;

    /*
     * Trap case of empty list
     */
    if (n == (struct br_node *)&(list->head))
        return NULL;

    list->tail = n->prev;

    return n;
}

/*
 * Add a node to list following the node 'here'
 */
void BR_PUBLIC_ENTRY BrInsert(struct br_list *list, struct br_node *here, struct br_node *node)
{
    node->next = here->next;
    node->prev = here;

    here->next->prev = node;
    here->next = node;
}

/*
 * Remove a node from the containing list
 */
struct br_node *BR_PUBLIC_ENTRY BrRemove(struct br_node *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;

    return node;
}

/**
 ** Simple list handling
 **/

void BR_PUBLIC_ENTRY BrSimpleNewList(struct br_simple_list *list)
{
    list->head = NULL;
}

void BR_PUBLIC_ENTRY BrSimpleAddHead(struct br_simple_list *list, struct br_simple_node *node)
{
    node->next = list->head;
    node->prev = &list->head;

    if (node->next)
        node->next->prev = &node->next;

    list->head = node;
}

struct br_simple_node *BR_PUBLIC_ENTRY BrSimpleRemHead(struct br_simple_list *list)
{
    struct br_simple_node *node = list->head;

    if (node)
        BrSimpleRemove(node);

    return node;
}

void BR_PUBLIC_ENTRY BrSimpleInsert(struct br_simple_list *list, struct br_simple_node *here,
                                    struct br_simple_node *node)
{
    node->next = here->next;
    node->prev = &here->next;

    if (node->next)
        node->next->prev = &node->next;

    here->next = node;
}

struct br_simple_node *BR_PUBLIC_ENTRY BrSimpleRemove(struct br_simple_node *node)
{
    *(node->prev) = node->next;

    if (node->next)
        node->next->prev = node->prev;

    node->prev = NULL;
    node->next = NULL;

    return node;
}
