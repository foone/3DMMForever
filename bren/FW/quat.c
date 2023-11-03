/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: quat.c 1.11 1995/02/22 21:42:28 sam Exp $
 * $Locker:  $
 *
 * Unit quaternion manipulation - based on
 *  "Quaternion Calculus and Fast Animation" Ken Shoemake
 *
 */

#include "fw.h"
#include "shortcut.h"

static char rscid[] = "$Id: quat.c 1.11 1995/02/22 21:42:28 sam Exp $";

br_quat *BR_PUBLIC_ENTRY BrQuatMul(br_quat *q, br_quat *l, br_quat *r)
{
#if 1
    /*
     * Quaternion multiply using 8 muls (but more adds)
     */
    br_scalar x1, x2, x3, x4, x5, x6, x7, x8;
    br_scalar s, t;

    x1 = BR_MUL((lZ - lY), (rY - rZ));
    x2 = BR_MUL((lW + lX), (rW + rX));
    x3 = BR_MUL((lW - lX), (rY + rZ));
    x4 = BR_MUL((lZ + lY), (rW - rX));

    x5 = BR_MUL((lZ - lX), (rX - rY));
    x6 = BR_MUL((lZ + lX), (rX + rY));
    x7 = BR_MUL((lW + lY), (rW - rZ));
    x8 = BR_MUL((lW - lY), (rW + rZ));

    s = x6 + x7 + x8;
    t = BR_CONST_DIV(x5 + s, 2);

    qW = x1 + t - x6;
    qX = x2 + t - s;
    qY = x3 + t - x8;
    qZ = x4 + t - x7;
#endif

#if 0
	br_quat t;

	/*
	 * Normal 16 muls
	 */
	tW = BR_MAC4(lW,rW,-lX,rX,-lY,rY,-lZ,rZ);
	tX = BR_MAC4(lW,rX, lX,rW, lY,rZ,-lZ,rY);
	tY = BR_MAC4(lW,rY, lY,rW, lZ,rX,-lX,rZ);
	tZ = BR_MAC4(lW,rZ, lZ,rW, lX,rY,-lY,rX);

	*q = t;
#endif

    return q;
}

br_quat *BR_PUBLIC_ENTRY BrQuatNormalise(br_quat *q, br_quat *qq)
{
    br_scalar s = BR_DIV(S1, BR_LENGTH4(qqX, qqY, qqZ, qqW));

    qX = BR_MUL(qqX, s);
    qY = BR_MUL(qqY, s);
    qZ = BR_MUL(qqZ, s);
    qW = BR_MUL(qqW, s);

    return q;
}

br_quat *BR_PUBLIC_ENTRY BrQuatInvert(br_quat *q, br_quat *qq)
{
    qX = -qqX;
    qY = -qqY;
    qZ = -qqZ;
    qW = qqW;

    return q;
}

br_quat *BR_PUBLIC_ENTRY BrQuatSlerp(br_quat *q, br_quat *l, br_quat *r, br_scalar a, br_int_16 spins)
{
    int omega, omega_spin;
    br_scalar s_omega, c_omega;
    br_scalar scale_l, scale_r;
    br_quat t;

    c_omega = BR_MAC4(lX, rX, lY, rY, lZ, rZ, lW, rW);

    /*
     * Take shortest route
     */
    if (c_omega < 0)
    {
        tX = -rX;
        tY = -rY;
        tZ = -rZ;
        tW = -rW;
        c_omega = -c_omega;
    }
    else
    {
        tX = rX;
        tY = rY;
        tZ = rZ;
        tW = rW;
    }

    if ((S1 - c_omega) > BR_SCALAR_EPSILON)
    {
        /*
         * Normal case
         */
        omega = BR_ACOS(c_omega);
        omega_spin = omega + spins * BR_ANGLE_RAD(PI);

        s_omega = BR_SIN((br_angle)omega);
        scale_l = BR_DIV(BR_SIN((br_angle)(omega - BR_MUL(a, omega_spin))), s_omega);
        scale_r = BR_DIV(BR_SIN((br_angle)BR_MUL(a, omega_spin)), s_omega);
    }
    else
    {
        /*
         * Ends very close
         */
        scale_l = S1 - a;
        scale_r = a;
    }

    qX = BR_MAC2(scale_l, lX, scale_r, tX);
    qY = BR_MAC2(scale_l, lY, scale_r, tY);
    qZ = BR_MAC2(scale_l, lZ, scale_r, tZ);
    qW = BR_MAC2(scale_l, lW, scale_r, tW);

    return q;
}

br_matrix34 *BR_PUBLIC_ENTRY BrQuatToMatrix34(br_matrix34 *mat, br_quat *q)
{
    br_scalar xs, ys, zs;
    br_scalar wx, wy, wz;
    br_scalar xx, xy, xz;
    br_scalar yy, yz, zz;

    /* NB: unit quat is assumed */

    xs = BR_CONST_MUL(qX, 2);
    ys = BR_CONST_MUL(qY, 2);
    zs = BR_CONST_MUL(qZ, 2);

    wx = BR_MUL(qW, xs);
    wy = BR_MUL(qW, ys);
    wz = BR_MUL(qW, zs);
    xx = BR_MUL(qX, xs);
    xy = BR_MUL(qX, ys);
    xz = BR_MUL(qX, zs);
    yy = BR_MUL(qY, ys);
    yz = BR_MUL(qY, zs);
    zz = BR_MUL(qZ, zs);

    M(0, 0) = S1 - (yy + zz);
    M(1, 0) = xy - wz;
    M(2, 0) = xz + wy;

    M(0, 1) = xy + wz;
    M(1, 1) = S1 - (xx + zz);
    M(2, 1) = yz - wx;

    M(0, 2) = xz - wy;
    M(1, 2) = yz + wx;
    M(2, 2) = S1 - (xx + yy);

    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;

    return mat;
}

br_quat *BR_PUBLIC_ENTRY BrMatrix34ToQuat(br_quat *q, br_matrix34 *mat)
{
    br_scalar tr, s;
    int i, j, k;

    tr = M(0, 0) + M(1, 1) + M(2, 2);

    if (tr >= S0)
    {
        s = BR_SQRT(tr + S1);
        qW = BR_CONST_DIV(s, 2);

        s = BR_DIV(BR_SCALAR(0.5), s);

        qX = BR_MUL(M(1, 2) - M(2, 1), s);
        qY = BR_MUL(M(2, 0) - M(0, 2), s);
        qZ = BR_MUL(M(0, 1) - M(1, 0), s);
    }
    else
    {
        static int n[] = {1, 2, 0};

        i = 0;
        if (M(1, 1) > M(0, 0))
            i = 1;
        if (M(2, 2) > M(i, i))
            i = 2;

        j = n[i];
        k = n[j];

        s = BR_SQRT(M(i, i) - (M(j, j) + M(k, k)) + S1);
        qi(i) = BR_CONST_DIV(s, 2);

        s = BR_DIV(BR_SCALAR(0.5), s);

        qW = BR_MUL(M(j, k) - M(k, j), s);
        qi(j) = BR_MUL(M(i, j) + M(j, i), s);
        qi(k) = BR_MUL(M(i, k) + M(k, i), s);
    }

    return q;
}

br_matrix4 *BR_PUBLIC_ENTRY BrQuatToMatrix4(br_matrix4 *mat, br_quat *q)
{
    br_matrix34 tmp;

    BrQuatToMatrix34(&tmp, q);
    BrMatrix4Copy34(mat, &tmp);

    return mat;
}

br_quat *BR_PUBLIC_ENTRY BrMatrix4ToQuat(br_quat *q, br_matrix4 *mat)
{
    br_matrix34 tmp;

    BrMatrix34Copy4(&tmp, mat);

    return BrMatrix34ToQuat(q, &tmp);
}
