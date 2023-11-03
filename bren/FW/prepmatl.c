/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: prepmatl.c 1.6 1995/02/22 21:42:24 sam Exp $
 * $Locker:  $
 *
 * Precompute information for materials
 */

#include "fw.h"
#include "brassert.h"
#include "shortcut.h"

static char rscid[] = "$Id: prepmatl.c 1.6 1995/02/22 21:42:24 sam Exp $";

void BR_PUBLIC_ENTRY BrMaterialUpdate(br_material *mat, br_uint_16 flags)
{
    mat->prep_flags &= ~(MATUF_SURFACE_VERTICES | MATUF_SURFACE_FACES);

    if (mat->flags & BR_MATF_SMOOTH)
        mat->prep_flags |= MATUF_SURFACE_VERTICES;
    else
        mat->prep_flags |= MATUF_SURFACE_FACES;

    if (mat->colour_map)
        mat->prep_flags |= MATUF_SURFACE_VERTICES;

    /*
     * Call any renderer function
     */
    if (fw.material_update)
        fw.material_update(mat, flags);
}
