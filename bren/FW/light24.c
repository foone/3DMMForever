/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: light24.c 1.5 1995/03/01 15:26:08 sam Exp $
 * $Locker:  $
 *
 * Bits of the lighting model for true colour pixels
 */

#include "fw.h"
#include "shortcut.h"
#include "brassert.h"

static char rscid[] = "$Id: light24.c 1.5 1995/03/01 15:26:08 sam Exp $";

/*
 * Evaluate the vertex lighting function, and return a value
 * for uniform face rendering
 */
br_uint_32 BR_SURFACE_CALL LightingFaceColour(br_vertex *v, br_face *fp, int reversed)
{
    br_scalar comp[NUM_COMPONENTS];
    br_fvector3 rev_normal;

    if (!reversed)
    {
        fw.surface_fn(v, &fp->n, comp);
    }
    else
    {

        BrFVector3Negate(&rev_normal, &fp->n);
        fw.surface_fn(v, &rev_normal, comp);
    }

    return BR_COLOUR_RGB(BrScalarToInt(comp[C_R]), BrScalarToInt(comp[C_G]), BrScalarToInt(comp[C_B]));
}

/*
 * Lighting function for unlit colour
 */
void BR_SURFACE_CALL LightingColourCopyMaterial(br_vertex *v, br_fvector3 *n, br_scalar *comp)
{
    comp[C_R] = BrIntToScalar(BR_RED(fw.material->colour));
    comp[C_G] = BrIntToScalar(BR_GRN(fw.material->colour));
    comp[C_B] = BrIntToScalar(BR_BLU(fw.material->colour));
}

/*
 * Lighting function for prelit colour
 */
void BR_SURFACE_CALL LightingColourCopyVertex(br_vertex *v, br_fvector3 *n, br_scalar *comp)
{
    comp[C_R] = BrIntToScalar(v->red);
    comp[C_G] = BrIntToScalar(v->grn);
    comp[C_B] = BrIntToScalar(v->blu);
}

/*
 * Work out 3 component lighting given:
 * 	A point
 *	A normal
 *  A material
 *
 * Examines all active lights and accumulates lighting from each light
 *
 * Writes results into
 *	comp[C_R]
 *	comp[C_G]
 *	comp[C_B]
 */
void BR_SURFACE_CALL LightingColour(br_vertex *v, br_fvector3 *n, br_scalar *comp)
{
    br_active_light *alp;
    int i;
    br_vector3 vp;
    br_vector3 vn;
    br_fvector3 fvn;

    br_scalar lcomp[NUM_L_COMPONENTS];
    br_scalar r, g, b;

    r = g = b = BR_SCALAR(0.0);

    fw.eye_l = fw.eye_m_normalised;
    alp = fw.active_lights_model;

    /*
     * Ambient component
     */
    r = BR_MUL(fw.material->ka, BrFixedToScalar(BR_RED(fw.material->colour) << 8));
    g = BR_MUL(fw.material->ka, BrFixedToScalar(BR_GRN(fw.material->colour) << 8));
    b = BR_MUL(fw.material->ka, BrFixedToScalar(BR_BLU(fw.material->colour) << 8));

    /*
     * Accumulate intensities for each active light in model space
     */
    alp = fw.active_lights_model;
    for (i = 0; i < fw.nactive_lights_model; i++, alp++)
    {
        alp->light_sub_function(&v->p, n, alp, lcomp);

        r += lcomp[L_R];
        g += lcomp[L_G];
        b += lcomp[L_B];
    }

    /*
     * See if any lights are to be calculated in view space
     */
    if (fw.nactive_lights_view)
    {
        BrMatrix34ApplyP(&vp, &v->p, &fw.model_to_view);
        BrMatrix34TApplyFV(&vn, n, &fw.view_to_model);
        BrFVector3Normalise(&fvn, &vn);

        fw.eye_l.v[0] = BR_SCALAR(0);
        fw.eye_l.v[1] = BR_SCALAR(0);
        fw.eye_l.v[2] = BR_SCALAR(1);

        /*
         * ... and accumulate
         */
        alp = fw.active_lights_view;
        for (i = 0; i < fw.nactive_lights_view; i++, alp++)
        {
            alp->light_sub_function(&vp, &fvn, alp, lcomp);
            r += lcomp[L_R];
            g += lcomp[L_G];
            b += lcomp[L_B];
        }
    }

    /*
     * Scale and clamp to final range
     */
    comp[C_R] = BR_MUL(r, BR_SCALAR(256));
    comp[C_G] = BR_MUL(g, BR_SCALAR(256));
    comp[C_B] = BR_MUL(b, BR_SCALAR(256));

#define RGB_MAX BR_SCALAR(254.0)
#define RGB_MIN BR_SCALAR(1.0)

    if (comp[C_R] >= RGB_MAX)
        comp[C_R] = RGB_MAX;
    else if (comp[C_R] < RGB_MIN)
        comp[C_R] = RGB_MIN;

    if (comp[C_G] >= RGB_MAX)
        comp[C_G] = RGB_MAX;
    else if (comp[C_G] < RGB_MIN)
        comp[C_G] = RGB_MIN;

    if (comp[C_B] >= RGB_MAX)
        comp[C_B] = RGB_MAX;
    else if (comp[C_B] < RGB_MIN)
        comp[C_B] = RGB_MIN;
}

/*
 * Lighting function that returns 0.0
 * True colour
 */
void LightingColour_Null(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    lcomp[L_R] = lcomp[L_G] = lcomp[L_B] = BR_SCALAR(1.0);
}

/*
 * Lighting for directional light source in model space
 * True colour
 */
void LightingColour_Dirn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    br_scalar c, l, d;

    /*
     * Accumulate diffuse and specular components
     *
     */

    /*
     * Diffuse (alp->direction already has alp->intensitt factored into it)
     */
    c = BrFVector3Dot(n, &alp->direction);

    if (c > BR_SCALAR(0.0))
    {

        c = BR_MUL(c, fw.material->kd);

        lcomp[L_R] = BR_MUL(c, BrFixedToScalar(BR_RED(fw.material->colour) << 8));
        lcomp[L_G] = BR_MUL(c, BrFixedToScalar(BR_GRN(fw.material->colour) << 8));
        lcomp[L_B] = BR_MUL(c, BrFixedToScalar(BR_BLU(fw.material->colour) << 8));

        if (fw.material->ks != BR_SCALAR(0.0))
        {

            c = BrFVector3Dot(n, &alp->half);

            if (c > SPECULARPOW_CUTOFF)
            {
                l = BR_MUL(fw.material->ks, alp->intensity);
                d = BR_MULDIV(c, l, fw.material->power - BR_MUL(fw.material->power, c) + c);

                lcomp[L_R] += d;
                lcomp[L_G] += d;
                lcomp[L_B] += d;
            }
        }

        lcomp[L_R] = BR_MUL(lcomp[L_R], BrFixedToScalar(BR_RED(alp->light->colour) << 8));
        lcomp[L_G] = BR_MUL(lcomp[L_G], BrFixedToScalar(BR_GRN(alp->light->colour) << 8));
        lcomp[L_B] = BR_MUL(lcomp[L_B], BrFixedToScalar(BR_BLU(alp->light->colour) << 8));
    }
    else
        lcomp[L_R] = lcomp[L_G] = lcomp[L_B] = BR_SCALAR(0.0);
}

/*
 * Lighting for point light source with attenuation
 * True colour
 */
void LightingColour_Point(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    br_scalar c, rd, l, d;
    br_vector3 dirn, dirn_norm, r, sn;

    /*
     * Work out vector between point and light source
     */
    BrVector3Sub(&dirn, &alp->position, p);
    BrVector3Normalise(&dirn_norm, &dirn);

    /*
     * Diffuse
     */

    c = BrFVector3Dot(n, &dirn_norm);

    if (c > BR_SCALAR(0.0))
    {

        c = BR_MUL(alp->intensity, BR_MUL(c, fw.material->kd));

        lcomp[L_R] = BR_MUL(c, BrFixedToScalar(BR_RED(fw.material->colour) << 8));
        lcomp[L_G] = BR_MUL(c, BrFixedToScalar(BR_GRN(fw.material->colour) << 8));
        lcomp[L_B] = BR_MUL(c, BrFixedToScalar(BR_BLU(fw.material->colour) << 8));

        if (fw.material->ks != BR_SCALAR(0.0))
        {
            /*
             * Specular
             */
            BrVector3CopyF(&sn, n);
            rd = BR_CONST_MUL(BrVector3Dot(&dirn_norm, &sn), 2);
            BrVector3Scale(&r, &sn, rd);
            BrVector3Sub(&r, &r, &dirn_norm);

            c = BrVector3Dot(&fw.eye_l, &r);

            /*
             * Phong lighting approximation from Gems IV pg. 385
             */
            if (c > SPECULARPOW_CUTOFF)
            {
                l = BR_MUL(fw.material->ks, alp->intensity);
                d = BR_MULDIV(c, l, fw.material->power - BR_MUL(fw.material->power, c) + c);

                lcomp[L_R] += d;
                lcomp[L_G] += d;
                lcomp[L_B] += d;
            }
        }

        lcomp[L_R] = BR_MUL(lcomp[L_R], BrFixedToScalar(BR_RED(alp->light->colour) << 8));
        lcomp[L_G] = BR_MUL(lcomp[L_G], BrFixedToScalar(BR_GRN(alp->light->colour) << 8));
        lcomp[L_B] = BR_MUL(lcomp[L_B], BrFixedToScalar(BR_BLU(alp->light->colour) << 8));
    }
    else
        lcomp[L_R] = lcomp[L_G] = lcomp[L_B] = BR_SCALAR(0.0);
}

/*
 * Lighting for point light source with attenuation
 * Colour
 */
void LightingColour_PointAttn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    br_scalar c, d, d2, rd, i, power, l;
    br_vector3 dirn, dirn_norm, r, sn;

    /*
     * Work out vector between point and light source
     */
    BrVector3Sub(&dirn, &alp->position, p);

    /*
     * Work out attenuation with distance and distance^2
     */
    d = BrVector3Length(&dirn);

    if (d == BR_SCALAR(0.0))
    {
        lcomp[L_R] = lcomp[L_G] = lcomp[L_B] = BR_SCALAR(0.0);
        return;
    }

    if (d >= BR_SCALAR(180.0))
        d2 = BR_SCALAR(32767.0);
    else
        d2 = BR_MUL(d, d);

    i = alp->light->attenuation_c + BR_MUL(d, alp->light->attenuation_l) + BR_MUL(d2, alp->light->attenuation_q);

    d = BR_RCP(d);

    BrVector3Scale(&dirn_norm, &dirn, d);

    /*
     * Diffuse
     */

    c = BrFVector3Dot(n, &dirn_norm);

    if (c > BR_SCALAR(0.0))
    {

        c = BR_MUL(alp->intensity, BR_MUL(c, fw.material->kd));

        lcomp[L_R] = BR_MUL(c, BrFixedToScalar(BR_RED(fw.material->colour) << 8));
        lcomp[L_G] = BR_MUL(c, BrFixedToScalar(BR_GRN(fw.material->colour) << 8));
        lcomp[L_B] = BR_MUL(c, BrFixedToScalar(BR_BLU(fw.material->colour) << 8));

        if (fw.material->ks != BR_SCALAR(0.0))
        {
            /*
             * Specular
             */
            BrVector3CopyF(&sn, n);
            rd = BR_CONST_MUL(BrVector3Dot(&dirn_norm, &sn), 2);
            BrVector3Scale(&r, &sn, rd);
            BrVector3Sub(&r, &r, &dirn_norm);

            c = BrVector3Dot(&fw.eye_l, &r);

            /*
             * Phong lighting approximation from Gems IV pg. 385
             */
            if (c > SPECULARPOW_CUTOFF)
            {
                l = BR_MUL(fw.material->ks, alp->intensity);
                d = BR_MULDIV(c, l, fw.material->power - BR_MUL(fw.material->power, c) + c);

                lcomp[L_R] += d;
                lcomp[L_G] += d;
                lcomp[L_B] += d;
            }
        }

        lcomp[L_R] = BR_MULDIV(lcomp[L_R], BrFixedToScalar(BR_RED(alp->light->colour) << 8), i);
        lcomp[L_G] = BR_MULDIV(lcomp[L_G], BrFixedToScalar(BR_GRN(alp->light->colour) << 8), i);
        lcomp[L_B] = BR_MULDIV(lcomp[L_B], BrFixedToScalar(BR_BLU(alp->light->colour) << 8), i);
    }
    else
        lcomp[L_R] = lcomp[L_G] = lcomp[L_B] = BR_SCALAR(0.0);
}

/*
 * Lighting for spot light source
 * True colour
 */
void LightingColour_Spot(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    br_scalar c, rd, i = alp->intensity, power, l, d;
    br_vector3 dirn, dirn_norm, r, sn;

    /*
     * Work out vector between point and light source
     */
    BrVector3Sub(&dirn, &alp->position, p);
    BrVector3Normalise(&dirn_norm, &dirn);

    /*
     * Check cutoff
     */
    c = BrVector3Dot(&dirn_norm, &alp->direction);

    if (c < alp->spot_cosine_outer)
    {
        lcomp[L_R] = lcomp[L_G] = lcomp[L_B] = BR_SCALAR(0.0);
        return;
    }

    /*
     * Falloff between inner and outer cones
     */
    if (c < alp->spot_cosine_inner)
        i = BR_MULDIV(i, alp->spot_cosine_outer - c, alp->spot_cosine_outer - alp->spot_cosine_inner);

    /*
     * Diffuse
     */

    c = BrFVector3Dot(n, &dirn_norm);

    if (c > BR_SCALAR(0.0))
    {

        c = BR_MUL(alp->intensity, BR_MUL(c, fw.material->kd));

        lcomp[L_R] = BR_MUL(c, BrFixedToScalar(BR_RED(fw.material->colour) << 8));
        lcomp[L_G] = BR_MUL(c, BrFixedToScalar(BR_GRN(fw.material->colour) << 8));
        lcomp[L_B] = BR_MUL(c, BrFixedToScalar(BR_BLU(fw.material->colour) << 8));

        if (fw.material->ks != BR_SCALAR(0.0))
        {
            /*
             * Specular
             */
            BrVector3CopyF(&sn, n);
            rd = BR_CONST_MUL(BrVector3Dot(&dirn_norm, &sn), 2);
            BrVector3Scale(&r, &sn, rd);
            BrVector3Sub(&r, &r, &dirn_norm);

            c = BrVector3Dot(&fw.eye_l, &r);

            /*
             * Phong lighting approximation from Gems IV pg. 385
             */
            if (c > SPECULARPOW_CUTOFF)
            {
                l = BR_MUL(fw.material->ks, alp->intensity);
                d = BR_MULDIV(c, l, fw.material->power - BR_MUL(fw.material->power, c) + c);

                lcomp[L_R] += d;
                lcomp[L_G] += d;
                lcomp[L_B] += d;
            }
        }

        lcomp[L_R] = BR_MUL(lcomp[L_R], BrFixedToScalar(BR_RED(alp->light->colour) << 8));
        lcomp[L_G] = BR_MUL(lcomp[L_G], BrFixedToScalar(BR_GRN(alp->light->colour) << 8));
        lcomp[L_B] = BR_MUL(lcomp[L_B], BrFixedToScalar(BR_BLU(alp->light->colour) << 8));

        lcomp[L_R] = BR_MUL(lcomp[L_R], i);
        lcomp[L_G] = BR_MUL(lcomp[L_G], i);
        lcomp[L_B] = BR_MUL(lcomp[L_B], i);
    }
    else
        lcomp[L_R] = lcomp[L_G] = lcomp[L_B] = BR_SCALAR(0.0);
}

/*
 * Lighting for spot light source with attenuation
 * True colour
 */
void LightingColour_SpotAttn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    br_scalar c, d, d2, rd, i, power, l;
    br_vector3 dirn, dirn_norm, r, sn;

    /*
     * Work out vector between point and light source
     */
    BrVector3Sub(&dirn, &alp->position, p);

    d = BrVector3Length(&dirn);

    if (d == BR_SCALAR(0.0))
    {
        lcomp[L_R] = lcomp[L_G] = lcomp[L_B] = BR_SCALAR(0.0);
        return;
    }

    d2 = BR_RCP(d);
    BrVector3Scale(&dirn_norm, &dirn, d2);

    /*
     * Check cutoff
     */
    c = BrVector3Dot(&dirn_norm, &alp->direction);

    if (c < alp->spot_cosine_outer)
    {
        lcomp[L_R] = lcomp[L_G] = lcomp[L_B] = BR_SCALAR(0.0);
        return;
    }

    /*
     * Work out attenuation with distance and distance^2
     */

    if (d == BR_SCALAR(0.0))
    {
        lcomp[L_R] = lcomp[L_G] = lcomp[L_B] = BR_SCALAR(0.0);
        return;
    }

    if (d >= BR_SCALAR(180.0))
        d2 = BR_SCALAR(32767.0);
    else
        d2 = BR_MUL(d, d);

    i = alp->light->attenuation_c + BR_MUL(d, alp->light->attenuation_l) + BR_MUL(d2, alp->light->attenuation_q);

    i = BR_RCP(i);

    /*
     * Falloff between inner and outer cones
     */
    if (c < alp->spot_cosine_inner)
        i = BR_MULDIV(i, alp->spot_cosine_outer - c, alp->spot_cosine_outer - alp->spot_cosine_inner);

    /*
     * Diffuse
     */

    c = BrFVector3Dot(n, &dirn_norm);

    if (c > BR_SCALAR(0.0))
    {

        c = BR_MUL(alp->intensity, BR_MUL(c, fw.material->kd));

        lcomp[L_R] = BR_MUL(c, BrFixedToScalar(BR_RED(fw.material->colour) << 8));
        lcomp[L_G] = BR_MUL(c, BrFixedToScalar(BR_GRN(fw.material->colour) << 8));
        lcomp[L_B] = BR_MUL(c, BrFixedToScalar(BR_BLU(fw.material->colour) << 8));

        if (fw.material->ks != BR_SCALAR(0.0))
        {
            /*
             * Specular
             */
            BrVector3CopyF(&sn, n);
            rd = BR_CONST_MUL(BrVector3Dot(&dirn_norm, &sn), 2);
            BrVector3Scale(&r, &sn, rd);
            BrVector3Sub(&r, &r, &dirn_norm);

            c = BrVector3Dot(&fw.eye_l, &r);

            /*
             * Phong lighting approximation from Gems IV pg. 385
             */
            if (c > SPECULARPOW_CUTOFF)
            {
                l = BR_MUL(fw.material->ks, alp->intensity);
                d = BR_MULDIV(c, l, fw.material->power - BR_MUL(fw.material->power, c) + c);

                lcomp[L_R] += d;
                lcomp[L_G] += d;
                lcomp[L_B] += d;
            }
        }

        lcomp[L_R] = BR_MUL(lcomp[L_R], BrFixedToScalar(BR_RED(alp->light->colour) << 8));
        lcomp[L_G] = BR_MUL(lcomp[L_G], BrFixedToScalar(BR_GRN(alp->light->colour) << 8));
        lcomp[L_B] = BR_MUL(lcomp[L_B], BrFixedToScalar(BR_BLU(alp->light->colour) << 8));

        lcomp[L_R] = BR_MUL(lcomp[L_R], i);
        lcomp[L_G] = BR_MUL(lcomp[L_G], i);
        lcomp[L_B] = BR_MUL(lcomp[L_B], i);
    }
    else
        lcomp[L_R] = lcomp[L_G] = lcomp[L_B] = BR_SCALAR(0.0);
}
