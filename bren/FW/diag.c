/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: diag.c 1.1 1995/07/28 19:03:10 sam Exp $
 * $Locker:  $
 *
 * Diagnostic handling support
 *
 * These routines are not called directly, but via macros. This is
 * to allow the underlying diagnostic mechanism to be radically changed
 * and allow various text scanning methods for message extraction
 *
 */

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "fw.h"

static char rscid[] = "$Id: diag.c 1.1 1995/07/28 19:03:10 sam Exp $";
static char _diag_scratch[128];

void BR_PUBLIC_ENTRY BrFailure(char *s, ...)
{
    va_list args;
    static char failure_header[] = "Failure: ";

    strcpy(_diag_scratch, failure_header);

    va_start(args, s);
    vsprintf(_diag_scratch + (BR_ASIZE(failure_header) - 1), s, args);
    va_end(args);

#if DEBUG
    if (fw.diag->failure == NULL)
    {
        fprintf(stderr, "FAILURE: NULL DIAGNOSTIC HANDLER: %s\n", _diag_scratch);
    }
#endif

    fw.diag->failure(_diag_scratch);
}

void BR_PUBLIC_ENTRY BrWarning(char *s, ...)
{
    va_list args;
    static char warning_header[] = "Warning: ";

    strcpy(_diag_scratch, warning_header);

    va_start(args, s);
    vsprintf(_diag_scratch + (BR_ASIZE(warning_header) - 1), s, args);
    va_end(args);

#if DEBUG
    if (fw.diag->warning == NULL)
    {
        fprintf(stderr, "WARNING: NULL DIAGNOSTIC HANDLER: %s\n", _diag_scratch);
    }
#endif

    fw.diag->warning(_diag_scratch);
}

void BR_PUBLIC_ENTRY BrFatal(char *name, int line, char *s, ...)
{
    va_list args;
    int n;

    n = sprintf(_diag_scratch, "FATAL %s:%d \n", name, line);
    va_start(args, s);
    vsprintf(_diag_scratch + n, s, args);
    va_end(args);

#if DEBUG
    if (fw.diag->failure == NULL)
    {
        fprintf(stderr, "FATAL: NULL DIAGNOSTIC HANDLER: %s\n", _diag_scratch);
    }
#endif

    fw.diag->failure(_diag_scratch);
}

#if DEBUG
void BR_PUBLIC_ENTRY _BrAssert(char *condition, char *file, unsigned line)
{
#if DEBUG
    if (fw.diag->failure == NULL)
    {
        fprintf(stderr, "ASSERT: NULL DIAGNOSTIC HANDLER: %s\n", _diag_scratch);
    }
#endif

    sprintf(_diag_scratch, "ASSERTION FAILED %s:%d: \"%s\"\n", file, line, condition);
    fw.diag->failure(_diag_scratch);
}
#endif

#if PARANOID
void BR_PUBLIC_ENTRY _BrUAssert(char *condition, char *file, unsigned line)
{
#if DEBUG
    if (fw.diag->failure == NULL)
    {
        fprintf(stderr, "UASSERT: NULL DIAGNOSTIC HANDLER: %s\n", _diag_scratch);
    }
#endif
    sprintf(_diag_scratch, "ASSERTION FAILED %s:%d: \"%s\"\n", file, line, condition);
    fw.diag->failure(_diag_scratch);
}
#endif
