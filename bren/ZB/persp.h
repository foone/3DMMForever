/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: persp.h 1.4 1995/08/31 16:47:38 sam Exp $
 * $Locker:  $
 *
 * Global scanline  workspace for persp. correct texture mapper
 */
#ifndef _PERSP_H_
#define _PERSP_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        char *start, *end;
        char *zstart;
        unsigned int denominator;
        int u_numerator, v_numerator;
        int du_numerator, dv_numerator;
        int dWx;
        int source;
        int x, y;
    } TexturedScanLine;

    extern TexturedScanLine BR_ASM_DATA tsl;

#ifdef __cplusplus
};
#endif
#endif
