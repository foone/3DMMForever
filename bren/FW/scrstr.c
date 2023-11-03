/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: scrstr.c 1.3 1995/05/25 13:24:06 sam Exp $
 * $Locker:  $
 *
 * A global scratchpad string used for printf and error operations
 */
#include "fw.h"

/*
 * Initialised to a value because IBM Cset++ LIB hides the symbol
 * otherwise ???
 */
char _br_scratch_string[512] = "SCRATCH";
