/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: error.c 1.5 1995/07/28 19:01:40 sam Exp $
 * $Locker:  $
 *
 * Error value support
 */

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "fw.h"

static char rscid[] = "$Id: error.c 1.5 1995/07/28 19:01:40 sam Exp $";

static br_error lastErrorType;
static void **lastErrorValue;

br_error BR_PUBLIC_ENTRY BrGetLastError(void **valuep)
{
    if (valuep)
        valuep = lastErrorValue;

    return lastErrorType;
}

void BrSetLastError(br_error type, void *value)
{
    lastErrorType = type;
    lastErrorValue = value;
}
