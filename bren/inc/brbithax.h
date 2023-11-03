/*
 *	NOTE(bjrkk): All of these are taken from either the BRender source code
 *  or from https://graphics.stanford.edu/~seander/bithacks.html
 */

#include <brender.h>

#ifndef _BRBITHAX_H_
#define _BRBITHAX_H_

static inline br_boolean __BrBitHaxIsPow2(br_uint_32 num)
{
    return num && !(num & (num - 1));
}

#endif
