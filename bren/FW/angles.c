/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: angles.c 1.12 1995/02/22 21:41:21 sam Exp $
 * $Locker:  $
 *
 * Euler angle manipulation - loosely based on -
 * 	"Euler Angle Convertion" Ken Shoemake, Graphics Gems IV pp. 222
 *
 */
#include "fw.h"
#include "brassert.h"
#include "shortcut.h"

static char rscid[] = "$Id: angles.c 1.12 1995/02/22 21:41:21 sam Exp $";

/*
 * Table of axes for each order
 */
static struct order_info
{
    br_uint_8 a0;
    br_uint_8 a1;
    br_uint_8 a2;
    br_uint_8 _pad[1];
} OrderAxes[] = {
    {0, 1, 2}, {1, 2, 0}, {2, 0, 1}, {0}, {0, 2, 1}, {1, 0, 2}, {2, 1, 0}, {0}, {0, 1, 2}, {1, 2, 0}, {2, 0, 1}, {0},
    {0, 2, 1}, {1, 0, 2}, {2, 1, 0}, {0}, {0, 1, 2}, {1, 2, 0}, {2, 0, 1}, {0}, {0, 2, 1}, {1, 0, 2}, {2, 1, 0}, {0},
    {0, 1, 2}, {1, 2, 0}, {2, 0, 1}, {0}, {0, 2, 1}, {1, 0, 2}, {2, 1, 0}, {0},
};

br_matrix34 *BR_PUBLIC_ENTRY BrEulerToMatrix34(br_matrix34 *mat, br_euler *euler)
{
    br_uint_8 o = eO;
    br_angle ti, tj, th;
    br_scalar ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
    int a0, a1, a2;

    UASSERT(mat != NULL);
    UASSERT(euler != NULL);
    UASSERT(euler->order < BR_ASIZE(OrderAxes));

    if (o & BR_EULER_FRAME)
        ti = eZ, tj = eY, th = eX;
    else
        ti = eX, tj = eY, th = eZ;

    if (o & BR_EULER_PARITY)
        ti = -ti, tj = -tj, th = -th;

    ci = BR_COS(ti);
    cj = BR_COS(tj);
    ch = BR_COS(th);
    si = BR_SIN(ti);
    sj = BR_SIN(tj);
    sh = BR_SIN(th);
    cc = BR_MUL(ci, ch);
    cs = BR_MUL(ci, sh);
    sc = BR_MUL(si, ch);
    ss = BR_MUL(si, sh);

    a0 = OrderAxes[o].a0;
    a1 = OrderAxes[o].a1;
    a2 = OrderAxes[o].a2;

    if (o & BR_EULER_REPEAT)
    {
        M(a0, a0) = cj;
        M(a1, a0) = BR_MUL(sj, si);
        M(a2, a0) = BR_MUL(sj, ci);
        M(a0, a1) = BR_MUL(sj, sh);
        M(a1, a1) = -BR_MUL(cj, ss) + cc;
        M(a2, a1) = -BR_MUL(cj, cs) - sc;
        M(a0, a2) = -BR_MUL(sj, ch);
        M(a1, a2) = BR_MUL(cj, sc) + cs;
        M(a2, a2) = BR_MUL(cj, cc) - ss;
    }
    else
    {
        M(a0, a0) = BR_MUL(cj, ch);
        M(a1, a0) = BR_MUL(sj, sc) - cs;
        M(a2, a0) = BR_MUL(sj, cc) + ss;
        M(a0, a1) = BR_MUL(cj, sh);
        M(a1, a1) = BR_MUL(sj, ss) + cc;
        M(a2, a1) = BR_MUL(sj, cs) - sc;
        M(a0, a2) = -sj;
        M(a1, a2) = BR_MUL(cj, si);
        M(a2, a2) = BR_MUL(cj, ci);
    }

    M(3, 0) = M(3, 1) = M(3, 2) = S0;

    return mat;
}

br_euler *BR_PUBLIC_ENTRY BrMatrix34ToEuler(br_euler *euler, br_matrix34 *mat)
{
    br_uint_8 o = eO;
    int a0, a1, a2;

    UASSERT(mat != NULL);
    UASSERT(euler != NULL);
    UASSERT(euler->order < BR_ASIZE(OrderAxes));

    a0 = OrderAxes[o].a0;
    a1 = OrderAxes[o].a1;
    a2 = OrderAxes[o].a2;

    if (o & BR_EULER_REPEAT)
    {
        br_scalar sy;

        sy = BR_LENGTH2(M(a1, a0), M(a2, a0));

        if (sy > BR_MUL(16, BR_SCALAR_EPSILON))
        {
            eX = BR_ATAN2(M(a1, a0), M(a2, a0));
            eY = BR_ATAN2(sy, M(a0, a0));
            eZ = BR_ATAN2(M(a0, a1), -M(a0, a2));
        }
        else
        {
            eX = BR_ATAN2(-M(a2, a1), M(a1, a1));
            eY = BR_ATAN2(sy, M(a0, a0));
            eZ = 0;
        }
    }
    else
    {
        br_scalar cy;

        cy = BR_LENGTH2(M(a0, a0), M(a0, a1));

        if (cy > BR_MUL(16, BR_SCALAR_EPSILON))
        {
            eX = BR_ATAN2(M(a1, a2), M(a2, a2));
            eY = BR_ATAN2(-M(a0, a2), cy);
            eZ = BR_ATAN2(M(a0, a1), M(a0, a0));
        }
        else
        {
            eX = BR_ATAN2(-M(a2, a1), M(a1, a1));
            eY = BR_ATAN2(-M(a0, a2), cy);
            eZ = 0;
        }
    }

    if (o & BR_EULER_PARITY)
    {
        eX = -eX;
        eY = -eY;
        eZ = -eZ;
    }

    if (o & BR_EULER_FRAME)
    {
        br_angle t;

        t = eX;
        eX = eZ;
        eZ = t;
    }

    return euler;
}

br_matrix4 *BR_PUBLIC_ENTRY BrEulerToMatrix4(br_matrix4 *mat, br_euler *euler)
{
    br_matrix34 tmp;

    UASSERT(mat != NULL);

    BrEulerToMatrix34(&tmp, euler);

    BrMatrix4Copy34(mat, &tmp);

    return mat;
}

br_euler *BR_PUBLIC_ENTRY BrMatrix4ToEuler(br_euler *dest, br_matrix4 *mat)
{
    br_matrix34 tmp;

    UASSERT(mat != NULL);

    BrMatrix34Copy4(&tmp, mat);

    return BrMatrix34ToEuler(dest, &tmp);
}

br_quat *BR_PUBLIC_ENTRY BrEulerToQuat(br_quat *q, br_euler *euler)
{
    br_uint_8 o = eO;
    br_angle ti, tj, th;
    br_scalar ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
    int a0, a1, a2;

    UASSERT(q != NULL);
    UASSERT(euler != NULL);
    UASSERT(euler->order < BR_ASIZE(OrderAxes));

    if (o & BR_EULER_FRAME)
        ti = eZ / 2, tj = eY / 2, th = eX / 2;
    else
        ti = eX / 2, tj = eY / 2, th = eZ / 2;

    if (o & BR_EULER_PARITY)
        tj = -tj;

    ci = BR_COS(ti);
    cj = BR_COS(tj);
    ch = BR_COS(th);
    si = BR_SIN(ti);
    sj = BR_SIN(tj);
    sh = BR_SIN(th);
    cc = BR_MUL(ci, ch);
    cs = BR_MUL(ci, sh);
    sc = BR_MUL(si, ch);
    ss = BR_MUL(si, sh);

    a0 = OrderAxes[o].a0;
    a1 = OrderAxes[o].a1;
    a2 = OrderAxes[o].a2;

    if (o & BR_EULER_REPEAT)
    {
        qi(a0) = BR_MUL(cj, (cs + sc));
        qi(a1) = BR_MUL(sj, (cc + ss));
        qi(a2) = BR_MUL(sj, (cs - sc));
        qi(3) = BR_MUL(cj, (cc - ss));
    }
    else
    {
        qi(a0) = BR_MUL(cj, sc) - BR_MUL(sj, cs);
        qi(a1) = BR_MUL(cj, ss) + BR_MUL(sj, cc);
        qi(a2) = BR_MUL(cj, cs) - BR_MUL(sj, sc);
        qi(3) = BR_MUL(cj, cc) + BR_MUL(sj, ss);
    }

    if (o & BR_EULER_PARITY)
        qi(a1) = -qi(a1);

    return q;
}

br_euler *BR_PUBLIC_ENTRY BrQuatToEuler(br_euler *euler, br_quat *q)
{
    br_matrix34 mat;

    /*
     * Convert quat to matrix
     */
    BrQuatToMatrix34(&mat, q);

    /*
     * Generate a matrix from the quat
     */
    return BrMatrix34ToEuler(euler, &mat);
}
