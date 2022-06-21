/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: light8.c 1.7 1995/03/29 16:41:16 sam Exp $
 * $Locker: sam $
 *
 * Bits of the lighting model for indexed pixels
 */

#include "fw.h"
#include "shortcut.h"
#include "brassert.h"

static char rscid[] = "$Id: light8.c 1.7 1995/03/29 16:41:16 sam Exp $";

/*
 * Evaluate the vertex lighting function, and return a value
 * for uniform face rendering
 */
br_uint_32 BR_SURFACE_CALL LightingFaceIndex(br_vertex *v, br_face *fp, int reversed)
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

    return BrScalarToInt(comp[C_I]);
}

/*
 * Lighting function for unlit indexed
 */
void BR_SURFACE_CALL LightingIndexCopyMaterial(br_vertex *v, br_fvector3 *n, br_scalar *comp)
{
    comp[C_I] = BrIntToScalar(fw.material->index_base);
}

/*
 * Lighting function for prelit indexed
 */
void BR_SURFACE_CALL LightingIndexCopyVertex(br_vertex *v, br_fvector3 *n, br_scalar *comp)
{
#if 0
	comp[C_I] = BrIntToScalar(v->index);
#else
    comp[C_I] = BR_MUL(fw.index_range, BR_CONST_DIV(BrIntToScalar(v->index), 256)) + fw.index_base;
#endif
}

/*
 * Accumulate lighting for multiple active lights by calling the
 * appropriate sub-function for each light
 *
 * Write the results into comp[C_I]
 */
void BR_SURFACE_CALL LightingIndex(br_vertex *v, br_fvector3 *n, br_scalar *comp)
{
    int i;
    br_active_light *alp;
    br_vector3 vp;
    br_vector3 vn;
    br_fvector3 fvn;

    br_scalar lcomp[NUM_L_COMPONENTS];
    br_scalar l;

    /*
     * Ambient component
     */
    l = fw.material->ka;

    /*
     * Accumulate intensities for each active light in model space
     */
    fw.eye_l = fw.eye_m_normalised;
    alp = fw.active_lights_model;
    for (i = 0; i < fw.nactive_lights_model; i++, alp++)
    {
        alp->light_sub_function(&v->p, n, alp, lcomp);
        l += lcomp[L_I];
    }

    /*
     * See if any lights are to be calculated in view space
     */
    if (fw.nactive_lights_view)
    {
        /*
         * Transform point and normal into view space ...
         */
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
            l += lcomp[L_I];
        }
    }

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

/*
 * Lighting function that returns 0.0
 * Index
 */
void LightingIndex_Null(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    lcomp[L_I] = BR_SCALAR(0.0);
}

/*
 * Lighting for directional light source in model space
 * Index
 */
void LightingIndex_Dirn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    br_scalar c, l, d;

    /*
     * Accumulate diffuse and specular components
     *
     */

    /*
     * Diffuse
     */
    c = BrFVector3Dot(n, &alp->direction);

    if (c > BR_SCALAR(0.0))
    {

        d = BR_MUL(c, fw.material->kd);

        if (fw.material->ks != BR_SCALAR(0.0))
        {
            /*
             * Specular
             */
            c = BrFVector3Dot(n, &alp->half);

            /*
             * Phong lighting approximation from Gems IV pg. 385
             */
            if (c > SPECULARPOW_CUTOFF)
            {
                l = BR_MUL(fw.material->ks, alp->intensity);
                d += BR_MULDIV(c, l, fw.material->power - BR_MUL(fw.material->power, c) + c);
            }
        }
        lcomp[L_I] = d;
    }
    else
        lcomp[L_I] = BR_SCALAR(0.0);
}

/*
 * Lighting for point light source with attenuation
 * Index
 */
void LightingIndex_Point(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    br_scalar c, rd, d;
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
        d = BR_MUL(c, fw.material->kd);

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
                d += BR_MULDIV(c, fw.material->ks, fw.material->power - BR_MUL(fw.material->power, c) + c);
        }
        lcomp[L_I] = BR_MUL(d, alp->intensity);
    }
    else
        lcomp[L_I] = BR_SCALAR(0.0);
}

/*
 * Lighting for point light source with attenuation
 * Index
 */
void LightingIndex_PointAttn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    br_scalar c, d2, rd, i;
    br_vector3 dirn, dirn_norm, r, sn;

    /*
     * Work out vector between point and light source
     */
    BrVector3Sub(&dirn, &alp->position, p);

    /*
     * Work out attenuation with distance and distance^2
     */
    lcomp[L_I] = BrVector3Length(&dirn);

    if (lcomp[L_I] == BR_SCALAR(0.0))
        return;

    if (lcomp[L_I] >= BR_SCALAR(180.0))
        d2 = BR_SCALAR(32767.0);
    else
        d2 = BR_MUL(lcomp[L_I], lcomp[L_I]);

    i = alp->light->attenuation_c + BR_MUL(lcomp[L_I], alp->light->attenuation_l) +
        BR_MUL(d2, alp->light->attenuation_q);

    lcomp[L_I] = BR_RCP(lcomp[L_I]);

    BrVector3Scale(&dirn_norm, &dirn, lcomp[L_I]);

    /*
     * Diffuse
     */

    c = BrFVector3Dot(n, &dirn_norm);

    if (c > BR_SCALAR(0.0))
    {
        lcomp[L_I] = BR_MUL(c, fw.material->kd);

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
                lcomp[L_I] += BR_MULDIV(c, fw.material->ks, fw.material->power - BR_MUL(fw.material->power, c) + c);
        }
        lcomp[L_I] = BR_DIV(lcomp[L_I], i);
    }
    else
        lcomp[L_I] = BR_SCALAR(0.0);
}

/*
 * Lighting for spot light source
 * Index
 */
void LightingIndex_Spot(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    br_scalar c, rd, i = alp->intensity;
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
        lcomp[L_I] = BR_SCALAR(0.0);
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
        lcomp[L_I] = BR_MUL(c, fw.material->kd);

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
                lcomp[L_I] += BR_MULDIV(c, fw.material->ks, fw.material->power - BR_MUL(fw.material->power, c) + c);
        }
        lcomp[L_I] = BR_MUL(lcomp[L_I], i);
    }
    else
        lcomp[L_I] = BR_SCALAR(0.0);
}

/*
 * Lighting for spot light source with attenuation
 * Index
 */
void LightingIndex_SpotAttn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp)
{
    br_scalar c, d2, rd, i;
    br_vector3 dirn, dirn_norm, r, sn;

    /*
     * Work out vector between point and light source
     */
    BrVector3Sub(&dirn, &alp->position, p);

    lcomp[L_I] = BrVector3Length(&dirn);

    if (lcomp[L_I] == BR_SCALAR(0.0))
        return;

    d2 = BR_RCP(lcomp[L_I]);
    BrVector3Scale(&dirn_norm, &dirn, d2);

    /*
     * Check cutoff
     */
    c = BrVector3Dot(&dirn_norm, &alp->direction);

    if (c < alp->spot_cosine_outer)
    {
        lcomp[L_I] = BR_SCALAR(0.0);
        return;
    }

    /*
     * Work out attenuation with distance and distance^2
     */

    if (lcomp[L_I] == BR_SCALAR(0.0))
        return;

    if (lcomp[L_I] >= BR_SCALAR(180.0))
        d2 = BR_SCALAR(32767.0);
    else
        d2 = BR_MUL(lcomp[L_I], lcomp[L_I]);

    i = alp->light->attenuation_c + +BR_MUL(lcomp[L_I], alp->light->attenuation_l) +
        BR_MUL(d2, alp->light->attenuation_q);

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
        lcomp[L_I] = BR_MUL(c, fw.material->kd);

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
                lcomp[L_I] += BR_MULDIV(c, fw.material->ks, fw.material->power - BR_MUL(fw.material->power, c) + c);
        }
        lcomp[L_I] = BR_MUL(lcomp[L_I], i);
    }
    else
        lcomp[L_I] = BR_SCALAR(0.0);
}
