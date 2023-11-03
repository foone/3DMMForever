/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: brhton.h 1.7 1995/05/25 13:37:25 sam Exp $
 * $Locker:  $
 *
 * Converting 'network' (big endian) byte order to host order
 *
 * This is a private implemenation - some platforms do not provide
 * this functionality. For those that do, this file should define
 * the operations in terms of the platform specific API
 */

#ifndef _BRHTON_H_
#define _BRHTON_H_

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * Mac is in network order already - little endian
     *
     * XXX SANE format
     */

#if BR_ENDIAN_BIG

    typedef float br_float;

#define BrHtoNL(x) (x)
#define BrNtoHL(x) (x)
#define BrHtoNS(x) (x)
#define BrNtoHS(x) (x)

#define BrHtoNF(x) (x)
#define BrNtoHF(x) (x)

#endif

/*
 * PC is little-endian
 */
#if BR_ENDIAN_LITTLE

#define BrNtoHL(x) BrSwap32(x)
#define BrHtoNL(x) BrSwap32(x)
#define BrNtoHS(x) BrSwap16(x)
#define BrHtoNS(x) BrSwap16(x)

#define BrHtoNF(x) BrSwapFloat(x)
#define BrNtoHF(x) BrSwapFloat(x)

#endif

#ifdef __cplusplus
};
#endif
#endif
