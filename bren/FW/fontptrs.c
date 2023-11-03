/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: fontptrs.c 1.2 1995/05/25 13:23:52 sam Exp $
 * $Locker:  $
 *
 * Public pointers to standard fonts
 */

#include "fw.h"

static char rscid[] = "$Id: fontptrs.c 1.2 1995/05/25 13:23:52 sam Exp $";

extern struct br_font BR_ASM_DATA _FontFixed3x5;
extern struct br_font BR_ASM_DATA _FontProp4x6;
extern struct br_font BR_ASM_DATA _FontProp7x9;

struct br_font *BrFontFixed3x5 = &_FontFixed3x5;
struct br_font *BrFontProp4x6 = &_FontProp4x6;
struct br_font *BrFontProp7x9 = &_FontProp7x9;
