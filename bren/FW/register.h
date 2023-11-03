/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: register.h 1.7 1995/03/16 11:57:08 sam Exp $
 * $Locker:  $
 *
 *
 */

#ifndef _REGISTER_H_
#define _REGISTER_H_

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * An entry in a registry - doubly linked list and pointers to data
     *
     * It is assumed that the first thing in the data structure is a pointer
     * to the item's name
     */
    typedef struct br_registry_entry
    {
        struct br_node node;
        char **item;
    } br_registry_entry;

    typedef void *BR_CALLBACK br_find_failed_cbfn(char *pattern);
    typedef br_uint_32 BR_CALLBACK br_enum_cbfn(void *item, void *arg);

    /*
     * Base structure for registry
     */
    typedef struct br_registery
    {
        /*
         * Anchor structure
         */
        struct br_list list;

        /*
         * Number of items in list
         */
        int count;

        /*
         * Hook that is called when RegistryFind fails
         */
        br_find_failed_cbfn *find_failed_hook;

    } br_registry;

#ifdef __cplusplus
};
#endif
#endif
