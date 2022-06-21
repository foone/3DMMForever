/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: prepmap.c 1.2 1995/02/22 21:42:23 sam Exp $
 * $Locker:  $
 *
 * Precompute information for texture maps
 */

#include "fw.h"
#include "brassert.h"
#include "shortcut.h"

static char rscid[] = "$Id: prepmap.c 1.2 1995/02/22 21:42:23 sam Exp $";

void BR_PUBLIC_ENTRY BrMapUpdate(br_pixelmap *map, br_uint_16 flags)
{
    /*
     * Call any renderer function
     */
    if (fw.map_update)
        fw.map_update(map, flags);
}
