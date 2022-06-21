/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: light8o.c 1.6 1995/03/01 15:26:09 sam Exp $
 * $Locker:  $
 *
 * Special case lighting models for indexed pixels
 */

#include "fw.h"
#include "shortcut.h"
#include "brassert.h"

static char rscid[] = "$Id: light8o.c 1.6 1995/03/01 15:26:09 sam Exp $";

/*
 * Special case surface function for when there is a single directional
 * light source in model space
 */
void BR_SURFACE_CALL LightingIndex_1MD(br_vertex *v, br_fvector3 *n, br_scalar *comp)
{
    int i;
    br_active_light *alp;
    br_vector3 vp;
    br_vector3 vn;
    br_fvector3 fvn;
    br_scalar l, s, c, d;

    /*
     * Diffuse
     */
    c = BrFVector3Dot(n, &fw.active_lights_model[0].direction);

    if (c > BR_SCALAR(0.0))
    {

        d = BR_MUL(c, fw.material->kd);

        if (fw.material->ks != BR_SCALAR(0.0))
        {

            /*
             * Specular
             */
            c = BrFVector3Dot(n, &fw.active_lights_model[0].half);

            /*
             * Phong lighting approximation from Gems IV pg. 385
             */
            if (c > SPECULARPOW_CUTOFF)
            {
                s = BR_MUL(fw.material->ks, fw.active_lights_model[0].intensity);
                d += BR_MULDIV(c, s, fw.material->power - BR_MUL(fw.material->power, c) + c);
            }
        }
        l = fw.material->ka + d;

        /*
         * Scale final intensity to range of indices
         *
         * XXX Could make the scale be shifts (only ^2 ranges)
         */

        if (l >= BR_SCALAR(1.0))
            comp[C_I] = fw.index_base + fw.index_range - BR_SCALAR_EPSILON;
        else if (l <= BR_SCALAR(0.0))
            comp[C_I] = fw.index_base;
        else
            comp[C_I] = BR_MUL(fw.index_range, l) + fw.index_base;
    }
    else
    {
        comp[C_I] = BR_MUL(fw.index_range, fw.material->ka) + fw.index_base;
    }
}

/*
 * Special case surface function for when there is a single directional
 * light source in model space and texture mapping
 */
void BR_SURFACE_CALL LightingIndex_1MDT(br_vertex *v, br_fvector3 *n, br_scalar *comp)
{
    int i;
    br_active_light *alp;
    br_vector3 vp;
    br_vector3 vn;
    br_fvector3 fvn;
    br_scalar l, s, c, d;

    /*
     * Diffuse
     */
    c = BrFVector3Dot(n, &fw.active_lights_model[0].direction);

    if (c > BR_SCALAR(0.0))
    {

        d = BR_MUL(c, fw.material->kd);

        if (fw.material->ks != BR_SCALAR(0.0))
        {

            /*
             * Specular
             */
            c = BrFVector3Dot(n, &fw.active_lights_model[0].half);

            /*
             * Phong lighting approximation from Gems IV pg. 385
             */
            if (c > SPECULARPOW_CUTOFF)
            {
                s = BR_MUL(fw.material->ks, fw.active_lights_model[0].intensity);
                d += BR_MULDIV(c, s, fw.material->power - BR_MUL(fw.material->power, c) + c);
            }
        }
        l = fw.material->ka + d;

        /*
         * Scale final intensity to range of indices
         *
         * XXX Could make the scale be shifts (only ^2 ranges)
         */

        if (l >= BR_SCALAR(1.0))
            comp[C_I] = fw.index_base + fw.index_range - BR_SCALAR_EPSILON;
        else if (l <= BR_SCALAR(0.0))
            comp[C_I] = fw.index_base;
        else
            comp[C_I] = BR_MUL(fw.index_range, l) + fw.index_base;
    }
    else
    {
        comp[C_I] = BR_MUL(fw.index_range, fw.material->ka) + fw.index_base;
    }

    APPLY_UV(comp[C_U], comp[C_V], v->map.v[U], v->map.v[V]);
}
