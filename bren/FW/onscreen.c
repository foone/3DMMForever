/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: onscreen.c 1.9 1995/02/22 21:42:16 sam Exp $
 * $Locker:  $
 *
 */
#include "fw.h"
#include "brassert.h"
#include "shortcut.h"

static char rscid[] = "$Id: onscreen.c 1.9 1995/02/22 21:42:16 sam Exp $";

/*
 * Check the model's bounding box against the view volume and return
 * one of:
 *
 *	OSC_REJECT	- Model is completely off screen
 *	OSC_PARTIAL	- Model is partially on screen
 *	OSC_ACCEPT  - Model is completely on screen
 *
 * Works by transforming the 6 planes that bound the view volume into
 * model space. We have the model_to_screen transfrom. To transform a
 * plane (screen to model), use the inverse of the transpose, ie: transpose
 * of model_to_screen. Since the view plane equations are just zero's
 * and ones, the transformed plane equation can be generated by adds/subs
 * of specific rows of the transform.
 *
 * To test the (axis aligned) box against the view plane, only two vertices
 * need testing - those that lie along the diagonal corresponding to the
 * direction of the plane's normal.
 *
 * The components of these test vertices are selected from min/max
 * by looking at the sign of the X,Y,Z components of the normal
 *
 * XXX Could probably write a mean version of this is assembler
 */

#define MAKE_EQN(sign, row)                                                                                            \
    {                                                                                                                  \
        eqn.v[X] = fw.model_to_screen.m[0][3] sign fw.model_to_screen.m[0][row];                                       \
        eqn.v[Y] = fw.model_to_screen.m[1][3] sign fw.model_to_screen.m[1][row];                                       \
        eqn.v[Z] = fw.model_to_screen.m[2][3] sign fw.model_to_screen.m[2][row];                                       \
        eqn.v[W] = -(fw.model_to_screen.m[3][3] sign fw.model_to_screen.m[3][row]);                                    \
    }

#define TEST_NOT_IN                                                                                                    \
    (BR_MAC3(eqn.v[X], ((eqn.v[X] > 0) ? (bounds->min.v[X]) : (bounds->max.v[X])), eqn.v[Y],                           \
             ((eqn.v[Y] > 0) ? (bounds->min.v[Y]) : (bounds->max.v[Y])), eqn.v[Z],                                     \
             ((eqn.v[Z] > 0) ? (bounds->min.v[Z]) : (bounds->max.v[Z]))) < eqn.v[W])

#define TEST_OUT                                                                                                       \
    (BR_MAC3(eqn.v[X], ((eqn.v[X] > 0) ? (bounds->max.v[X]) : (bounds->min.v[X])), eqn.v[Y],                           \
             ((eqn.v[Y] > 0) ? (bounds->max.v[Y]) : (bounds->min.v[Y])), eqn.v[Z],                                     \
             ((eqn.v[Z] > 0) ? (bounds->max.v[Z]) : (bounds->min.v[Z]))) < eqn.v[W])

br_uint_8 BR_PUBLIC_ENTRY BrOnScreenCheck(br_bounds *bounds)
{
    int accept = 1;
    br_vector4 eqn;
    int c;

    UASSERT(fw.rendering);

    /*
     * Left	  - screen space plane eqn. = ( 1, 0, 0, 1)
     */
    MAKE_EQN(+, 0);

    if (TEST_OUT)
        return OSC_REJECT;

    if (TEST_NOT_IN)
        accept = 0;

    /*
     * Right  - screen space plane eqn. = (-1, 0, 0, 1)
     */
    MAKE_EQN(-, 0);

    if (TEST_OUT)
        return OSC_REJECT;

    if (accept && TEST_NOT_IN)
        accept = 0;

    /*
     * Bottom - screen space plane eqn. = ( 0, 1, 0, 1)
     */
    MAKE_EQN(+, 1);

    if (TEST_OUT)
        return OSC_REJECT;

    if (accept && TEST_NOT_IN)
        accept = 0;

    /*
     * Top    - screen space plane eqn. = ( 0,-1, 0, 1)
     */
    MAKE_EQN(-, 1);

    if (TEST_OUT)
        return OSC_REJECT;

    if (accept && TEST_NOT_IN)
        accept = 0;

    /*
     * Yon    - screen space plane eqn. = ( 0, 0, 1, 1)
     */
    MAKE_EQN(+, 2);

    if (TEST_OUT)
        return OSC_REJECT;

    if (accept && TEST_NOT_IN)
        accept = 0;

    /*
     * Hither - screen space plane eqn. = ( 0, 0,-1, 0)
     */
    eqn.v[X] = -fw.model_to_screen.m[0][2];
    eqn.v[Y] = -fw.model_to_screen.m[1][2];
    eqn.v[Z] = -fw.model_to_screen.m[2][2];
    eqn.v[W] = fw.model_to_screen.m[3][2];

    if (TEST_OUT)
        return OSC_REJECT;

    if (accept && TEST_NOT_IN)
        accept = 0;

    /*
     * Any user clip planes
     */
    for (c = 0; c < fw.nactive_clip_planes; c++)
    {
        BrMatrix4TApply(&eqn, &fw.active_clip_planes[c].screen_plane, &fw.model_to_screen);
        eqn.v[W] = -eqn.v[W];

        if (TEST_OUT)
            return OSC_REJECT;

        if (accept && TEST_NOT_IN)
            accept = 0;
    }

    return accept ? OSC_ACCEPT : OSC_PARTIAL;
}