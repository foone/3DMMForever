/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: light24o.c 1.5 1995/02/22 21:41:59 sam Exp $
 * $Locker:  $
 *
 * Special case lighting models for true colour pixels
 */

#include "fw.h"
#include "shortcut.h"
#include "brassert.h"

static char rscid[] = "$Id: light24o.c 1.5 1995/02/22 21:41:59 sam Exp $";

/*
 * Special case surface function for when there is a single directional
 * light source in model space
 */
void BR_SURFACE_CALL LightingColour_1MD(br_vertex *v, br_fvector3 *n, br_scalar *comp)
{
    LightingColour(v, n, comp);
}

void BR_SURFACE_CALL LightingColour_1MDT(br_vertex *v, br_fvector3 *n, br_scalar *comp)
{
    LightingColour(v, n, comp);

    APPLY_UV(comp[C_U], comp[C_V], v->map.v[U], v->map.v[V]);
}
