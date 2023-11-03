/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: scalar.c 1.2 1995/02/22 21:42:36 sam Exp $
 * $Locker:  $
 *
 * Symbols that are defined to make sure apps. link to the right library
 */
#include "fw.h"

#if BASED_FIXED
int _BR_Fixed_Point_Scalar = 0;
#endif

#if BASED_FLOAT
int _BR_Floating_Point_Scalar = 0;
#endif
