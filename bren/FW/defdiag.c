/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: defdiag.c 1.1 1995/07/28 19:03:09 sam Exp $
 * $Locker:  $
 *
 * Default diagnostic handler that does nothing
 */
#include <brender.h>

static void BrNullWarning(char *message)
{
}

static void BrNullFailure(char *message)
{
}

/*
 * DiagHandler structure
 */
br_errorhandler BrNullDiagHandler = {
    "Null DiagHandler",
    BrNullWarning,
    BrNullFailure,
};

/*
 * Global variable that can be overridden by linking something first
 */
br_diaghandler *_BrDefaultDiagHandler = &BrNullDiagHandler;
