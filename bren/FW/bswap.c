/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: bswap.c 1.12 1995/05/25 13:23:11 sam Exp $
 * $Locker:  $
 *
 * Byte swap routines for net<->host conversion
 */

#include "fw.h"
#include "brassert.h"
#include "brhton.h"

static char rscid[] = "$Id: bswap.c 1.12 1995/05/25 13:23:11 sam Exp $";

#ifndef __IBMC__

#undef _SWAP
#define _SWAP(a, b)                                                                                                    \
    {                                                                                                                  \
        (a) ^= (b);                                                                                                    \
        (b) ^= (a);                                                                                                    \
        (a) ^= (b);                                                                                                    \
    }

/*
 * Swap the bytes of a 4 byte word
 */
br_uint_32 BR_PUBLIC_ENTRY BrSwap32(br_uint_32 l)
{
    union {
        unsigned long l;
        unsigned char c[4];
    } u;

    u.l = l;

    _SWAP(u.c[0], u.c[3]);
    _SWAP(u.c[1], u.c[2]);

    return u.l;
}

/*
 * Swap the bytes of a 2 byte word
 */
br_uint_16 BR_PUBLIC_ENTRY BrSwap16(br_uint_16 s)
{
    union {
        unsigned short s;
        unsigned char c[2];
    } u;

    u.s = s;

    _SWAP(u.c[0], u.c[1]);

    return u.s;
}

/*
 * Swap the bytes of a 4 byte float
 */
br_float BR_PUBLIC_ENTRY BrSwapFloat(br_float f)
{
    union {
        br_float f;
        unsigned char c[4];
    } u;

    u.f = f;

    _SWAP(u.c[0], u.c[3]);
    _SWAP(u.c[1], u.c[2]);

    return u.f;
}

/*
 * Swap an array of items
 *
 */
void *BR_PUBLIC_ENTRY BrSwapBlock(void *block, int count, int size)
{
    br_uint_8 *cp;
    int i, k;

    ASSERT(block != NULL);
    ASSERT(count != 0);
    ASSERT(size != 0);

    /*
     * Go through block swapping elements of the appropriate size
     */
    switch (size)
    {
    case 1:
        break;

        /*
         * Special case loops for the common sizes
         */
    case 2:
        for (i = 0, cp = block; i < count; i++, cp += 2)
        {
            _SWAP(cp[0], cp[1]);
        }
        break;

    case 3:
        for (i = 0, cp = block; i < count; i++, cp += 3)
        {
            _SWAP(cp[0], cp[2]);
        }
        break;

    case 4:
        for (i = 0, cp = block; i < count; i++, cp += 4)
        {
            _SWAP(cp[0], cp[3]);
            _SWAP(cp[1], cp[2]);
        }
        break;

    case 8:
        for (i = 0, cp = block; i < count; i++, cp += 8)
        {
            _SWAP(cp[0], cp[7]);
            _SWAP(cp[1], cp[6]);
            _SWAP(cp[2], cp[5]);
            _SWAP(cp[3], cp[4]);
        }
        break;

    default:
        /*
         * Do any other strange size the long way
         */
        for (i = 0, cp = block; i < count; i++, cp += size)
        {
            for (k = 0; k < size / 2; k++)
                _SWAP(cp[k], cp[size - k - 1]);
        }
        break;
    }

    return block;
}

#else

/*
 * Pull swap ops. from a BRender specific DLL
 */
br_uint_32 _System BrSupport2(br_uint_32 l);
br_uint_16 _System BrSupport3(br_uint_16 s);
br_float _System BrSupport4(br_float f);
void *_System BrSupport5(void *block, int count, int size);

br_uint_32 BR_PUBLIC_ENTRY BrSwap32(br_uint_32 l)
{
    return BrSupport2(l);
}

br_uint_16 BR_PUBLIC_ENTRY BrSwap16(br_uint_16 s)
{
    return BrSupport3(s);
}

br_float BR_PUBLIC_ENTRY BrSwapFloat(br_float f)
{
    return BrSupport4(f);
}

void *BR_PUBLIC_ENTRY BrSwapBlock(void *block, int count, int size)
{
    return BrSupport5(block, count, size);
}

#endif
