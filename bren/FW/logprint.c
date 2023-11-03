/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: logprint.c 1.5 1995/03/01 15:26:10 sam Exp $
 * $Locker:  $
 *
 * Low level wrappers for file system access
 */

#include <stdarg.h>
#include <stdio.h>

#include "fw.h"
#include "shortcut.h"

#if DEBUG

static char rscid[] = "$Id: logprint.c 1.5 1995/03/01 15:26:10 sam Exp $";

int BrLogPrintf(char *fmt, ...)
{
    int n;
    va_list args;

    va_start(args, fmt);
    n = vsprintf(_br_scratch_string, fmt, args);
    va_end(args);

    fwrite(_br_scratch_string, 1, n, stderr);

    return n;
}
#endif
