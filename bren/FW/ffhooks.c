/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: ffhooks.c 1.1 1995/02/26 20:08:28 sam Exp $
 * $Locker:  $
 *
 * FindFailed hooks that load models/materials/maps/tables from the filesystem
 */
#include <stdlib.h>

#include "brender.h"

/*
 * Find Failed callbacks to automatically load textures & tables
 */
br_pixelmap *BR_CALLBACK BrMapFindFailedLoad(char *name)
{
    br_pixelmap *pm;

    if ((pm = BrPixelmapLoad(name)) != NULL)
    {
        pm->identifier = BrMemStrDup(name);
        BrMapAdd(pm);
    }

    return pm;
}

br_pixelmap *BR_CALLBACK BrTableFindFailedLoad(char *name)
{
    br_pixelmap *pm;

    if ((pm = BrPixelmapLoad(name)) != NULL)
    {
        pm->identifier = BrMemStrDup(name);
        BrTableAdd(pm);
    }

    return pm;
}

br_model *BR_CALLBACK BrModelFindFailedLoad(char *name)
{
    br_model *m;

    if ((m = BrModelLoad(name)) != NULL)
    {
        m->identifier = BrMemStrDup(name);
        BrModelAdd(m);
    }

    return m;
}

br_material *BR_CALLBACK BrMaterialFindFailedLoad(char *name)
{
    br_material *m;

    if ((m = BrMaterialLoad(name)) != NULL)
    {
        m->identifier = BrMemStrDup(name);
        BrMaterialAdd(m);
    }

    return m;
}
