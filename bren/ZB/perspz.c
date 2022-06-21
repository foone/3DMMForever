/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: perspz.c 1.1 1995/08/31 16:52:50 sam Exp $
 * $Locker:  $
 *
 * Perspective texture mapper
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <brbuiltin.h>

#define FIX 1

#include "zb.h"
#include "shortcut.h"
#include "brassert.h"

#define swap(type, a, b)                                                                                               \
    {                                                                                                                  \
        type _;                                                                                                        \
        _ = a;                                                                                                         \
        a = b;                                                                                                         \
        b = _;                                                                                                         \
    }

extern float frcp[100];

extern int dither[4][4];

extern void BR_ASM_CALL ScanLinePIZ2TIP256(void);
extern void BR_ASM_CALL ScanLinePIZ2TIP64(void);
extern void BR_ASM_CALL ScanLinePIZ2TIP1024(void);
extern void BR_ASM_CALL ScanLinePIZ2TP256(void);
extern void BR_ASM_CALL ScanLinePIZ2TP64(void);
extern void BR_ASM_CALL ScanLinePIZ2TP1024(void);
extern void BR_ASM_CALL ScanLinePIZ2TPD1024(void);

extern int BR_ASM_CALL SafeFixedMac2Div(int, int, int, int, int);

#define cutoff 4.0

#ifdef __WATCOMC__

extern int sar16(int);

extern int inch(int);
extern int incl(int);
extern int dech(int);
extern int decl(int);

#pragma aux sar16 = "sar eax,16" parm[eax]

#pragma aux inch = "inc ah" parm[eax]
#pragma aux incl = "inc al" parm[eax]
#pragma aux dech = "dec ah" parm[eax]
#pragma aux decl = "dec al" parm[eax]

#else

#define sar16(num) (br_int_32)(br_int_16)((br_uint_32)(num) >> 16)

#endif

#define ZB 1

#define CHEAT 1
#define FNAME TriangleRenderPIZ2TIP256
#define SNAME ScanLinePIZ2TIP256
#define SIZE 256
#define LIGHT 1
#define LINEAR TriangleRenderPIZ2TI
#include "perspi.inc.c"
#undef LINEAR
#undef LIGHT
#undef SIZE
#undef SNAME
#undef FNAME

#define FNAME TriangleRenderPIZ2TP256
#define SNAME ScanLinePIZ2TP256
#define SIZE 256
#define LIGHT 0
#define LINEAR TriangleRenderPIZ2T
#include "perspi.inc.c"
#undef LINEAR
#undef LIGHT
#undef SIZE
#undef FNAME
#undef SNAME

#define FNAME TriangleRenderPIZ2TIP64
#define SNAME ScanLinePIZ2TIP64
#define SIZE 64
#define LIGHT 1
#define LINEAR TriangleRenderPIZ2TIA
#include "perspi.inc.c"
#undef LINEAR
#undef LIGHT
#undef SIZE
#undef SNAME
#undef FNAME

#define FNAME TriangleRenderPIZ2TP64
#define SNAME ScanLinePIZ2TP64
#define SIZE 64
#define LIGHT 0
#define LINEAR TriangleRenderPIZ2TA
#include "perspi.inc.c"
#undef LINEAR
#undef LIGHT
#undef SIZE
#undef SNAME
#undef FNAME

#define FNAME TriangleRenderPIZ2TP1024
#define SNAME ScanLinePIZ2TP1024
#define SIZE 1024
#define LIGHT 0
#define LINEAR TriangleRenderPIZ2TA
#include "perspi.inc.c"
#undef LINEAR
#undef LIGHT
#undef SIZE
#undef SNAME
#undef FNAME

#define FNAME TriangleRenderPIZ2TIP1024
#define SNAME ScanLinePIZ2TIP1024
#define SIZE 1024
#define LIGHT 1
#define LINEAR TriangleRenderPIZ2TIA
#include "perspi.inc.c"
#undef LINEAR
#undef LIGHT
#undef SIZE
#undef SNAME
#undef FNAME
#undef CHEAT

/*
 * Turn off cheating simply because we have no linear
 * dithered 1024x1024 renderer
 */
#define CHEAT 0
#define FNAME TriangleRenderPIZ2TPD1024
#define SNAME ScanLinePIZ2TPD1024
#define SIZE 1024
#define LIGHT 0
#include "perspi.inc.c"
#undef LINEAR
#undef LIGHT
#undef SIZE
#undef SNAME
#undef FNAME