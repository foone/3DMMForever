/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: brassert.c 1.3 1995/05/25 13:23:08 sam Exp $
 * $Locker:  $
 *
 * Assertion support routines
 */
#include <stdio.h>
#include <string.h>

#if DEBUG
void _BrAssert(char *condition, char *file, unsigned line)
{
    _ErrorOutput();
    printf("ASSERTION FAILED %s:%d: \"%s\"\n", file, line, condition);
    _ErrorExit(10);
}
#endif

#if PARANOID

void _BrUAssert(char *condition, char *file, unsigned line)
{
    _ErrorOutput();
    printf("USER ASSERTION FAILED %s:%d: \"%s\"\n", file, line, condition);
    _ErrorExit(10);
}
#endif
