/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: token.c 1.1 1995/07/28 19:03:11 sam Exp $
 * $Locker:  $
 *
 * Token allocation and debugging
 */
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "fw.h"
#include "brassert.h"

/**
 ** Basic token database support
 **/

br_token BR_PUBLIC_ENTRY BrTokenFind(char *pattern)
{
}

br_token BR_PUBLIC_ENTRY BrTokenFindMany(char *pattern)
{
}

br_token BR_PUBLIC_ENTRY BrTokenAdd(char *name, br_token value_hint)
{
}

#if 0
int BR_PUBLIC_ENTRY BrTokenAddMany(br_token *result, int result_max);
{
}
#endif

int BR_PUBLIC_ENTRY BrTokenRemove(br_token)
{
}

#if 0
int BR_PUBLIC_ENTRY BrTokenRemoveMany();
{
}
#endif

void BR_PUBLIC_ENTRY BrTokenEnum(br_token)
{
}

int BR_PUBLIC_ENTRY BrTokenCount(char *pattern)
{
}

/*
 * Support functions
 */
/*
 * Convert a string to a token/value list
 */

br_uint_32 *BR_PUBLIC_ENTRY BrTokenFromString(char *string)
{
}

br_uint_32 *BR_PUBLIC_ENTRY BrTokenToString(br_token *token)
{
}

br_uint_32 BR_PUBLIC_ENTRY BrTokenListExtract(br_token *list, br_token t)
{
}

int BR_PUBLIC_ENTRY BrTokenListExtractMany(br_token *list, ...)
{
}
