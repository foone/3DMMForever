/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: envmap.c 1.11 1995/03/01 15:25:57 sam Exp $
 * $Locker:  $
 *
 * Bits of the environment mapping support
 */
#include "fw.h"
#include "brassert.h"
#include "shortcut.h"

static char rscid[] = "$Id: envmap.c 1.11 1995/03/01 15:25:57 sam Exp $";

/*
 * Sets the new environment anchor
 *
 * Returns the previous value
 */
br_actor *BR_PUBLIC_ENTRY BrEnvironmentSet(br_actor *a)
{
    br_actor *old_a = fw.enabled_environment;

    fw.enabled_environment = a;

    return old_a;
}

/*
 * Generate U,V for environment assuming infinite eye
 */
void BR_SURFACE_CALL MapEnvironmentInfinite2D(br_vertex *v, br_fvector3 *normal, br_scalar *comp)
{
    br_vector3 r, wr, n;
    br_scalar d, cu, cv;
    br_angle a;

    /*
     * Convert normal to standard vector
     */
    n.v[0] = BrFractionToScalar(normal->v[0]);
    n.v[1] = BrFractionToScalar(normal->v[1]);
    n.v[2] = BrFractionToScalar(normal->v[2]);

    /*
     * Generate reflected vector
     *
     * -    - - -  -
     * R = 2N(N.E)-E
     */
    d = BR_CONST_MUL(BrVector3Dot(&fw.eye_m_normalised, &n), 2);
    BrVector3Scale(&r, &n, d);
    BrVector3Sub(&r, &r, &fw.eye_m_normalised);

    /*
     * If there is an environment frame, rotate vector into it
     */
    if (fw.enabled_environment)
    {
        BrMatrix34ApplyV(&wr, &r, &fw.model_to_environment);
        BrVector3Normalise(&wr, &wr);
    }
    else
        wr = r;

    /*
     * Convert vector to environment coordinates
     */
    cu = BrAngleToScalar(BR_ATAN2(wr.v[0], -wr.v[2]));

#if 0
	a = BR_ASIN(-wr.v[1]/2+BR_SCALAR(0.5));
	cv = BrAngleToScalar(a);
#else
    cv = -wr.v[1] / 2 + BR_SCALAR(0.5);
#endif

    APPLY_UV(comp[C_U], comp[C_V], cu, cv);

    /*
     * Call next surface function
     */
    fw.surface_fn_after_map(v, normal, comp);
}

/*
 * Generate U,V for environment assuming local eye
 */
void BR_SURFACE_CALL MapEnvironmentLocal2D(br_vertex *v, br_fvector3 *normal, br_scalar *comp)
{
    br_vector3 eye;
    br_vector3 r, wr, n;
    br_scalar d, cu, cv;
    br_angle a;

    /*
     * Generate eye vector -
     */
    BrVector3Sub(&eye, &fw.eye_m, &v->p);
    BrVector3Normalise(&eye, &eye);

    /*
     * Convert normal to standard vector
     */
    n.v[0] = BrFractionToScalar(normal->v[0]);
    n.v[1] = BrFractionToScalar(normal->v[1]);
    n.v[2] = BrFractionToScalar(normal->v[2]);

    /*
     * Generate reflected vector
     *
     * -    - - -  -
     * R = 2N(N.E)-E
     */
    d = BR_CONST_MUL(BrVector3Dot(&eye, &n), 2);
    BrVector3Scale(&r, &n, d);
    BrVector3Sub(&r, &r, &eye);

    /*
     * If there is an environment frame, rotate vector into it
     */
    if (fw.enabled_environment)
    {
        BrMatrix34ApplyV(&wr, &r, &fw.model_to_environment);
        BrVector3Normalise(&wr, &wr);
    }
    else
        wr = r;

    /*
     * Convert vector to environment coordinates
     */
    cu = BrAngleToScalar(BR_ATAN2(wr.v[0], -wr.v[2]));

#if 0
	a = BR_ASIN(-wr.v[1]/2+BR_SCALAR(0.5));
	cv = BrAngleToScalar(a);
#else
    cv = -wr.v[1] / 2 + BR_SCALAR(0.5);
#endif

    APPLY_UV(comp[C_U], comp[C_V], cu, cv);

    /*
     * Call next surface function
     */
    fw.surface_fn_after_map(v, normal, comp);
}

/*
 * Take U,V from vertex
 */
void BR_SURFACE_CALL MapFromVertex(br_vertex *v, br_fvector3 *normal, br_scalar *comp)
{
    APPLY_UV(comp[C_U], comp[C_V], v->map.v[U], v->map.v[V]);

    /*
     * Call next surface function
     */
    fw.surface_fn_after_map(v, normal, comp);
}

void BR_SURFACE_CALL MapFromVertexOnly(br_vertex *v, br_fvector3 *normal, br_scalar *comp)
{
    APPLY_UV(comp[C_U], comp[C_V], v->map.v[U], v->map.v[V]);
}
