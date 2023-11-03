/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: preptab.c 1.2 1995/02/22 21:42:26 sam Exp $
 * $Locker:  $
 *
 * Precompute information for tables
 */

#include "fw.h"
#include "brassert.h"
#include "shortcut.h"

static char rscid[] = "$Id: preptab.c 1.2 1995/02/22 21:42:26 sam Exp $";

void BR_PUBLIC_ENTRY BrTableUpdate(br_pixelmap *table, br_uint_16 flags)
{
    /*
     * Call any renderer function
     */
    if (fw.table_update)
        fw.table_update(table, flags);
}
