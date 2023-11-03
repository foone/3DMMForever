/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: vector.c 1.10 1995/08/31 16:29:49 sam Exp $
 * $Locker:  $
 *
 * Vector op. - some of these are redefined as macros in vecfns.h
 * for speed. (but the real functions still exist for debugging purposes)
 *
 */

#undef _NO_VECTOR_MACROS
#define _NO_VECTOR_MACROS 1

#include "fw.h"
#include "shortcut.h"

static char rscid[] = "$Id: vector.c 1.10 1995/08/31 16:29:49 sam Exp $";

/**
 ** 2D Vectors
 **/
void BR_PUBLIC_ENTRY BrVector2Copy(br_vector2 *v1, br_vector2 *v2)
{
    v1->v[0] = v2->v[0];
    v1->v[1] = v2->v[1];
}

/*
 * v1=(s1,s2)
 */
void BR_PUBLIC_ENTRY BrVector2Set(br_vector2 *v1, br_scalar s1, br_scalar s2)
{
    v1->v[0] = s1;
    v1->v[1] = s2;
}

void BR_PUBLIC_ENTRY BrVector2SetInt(br_vector2 *v1, int i1, int i2)
{
    v1->v[0] = BrIntToScalar(i1);
    v1->v[1] = BrIntToScalar(i2);
}

void BR_PUBLIC_ENTRY BrVector2SetFloat(br_vector2 *v1, float f1, float f2)
{
    v1->v[0] = BrFloatToScalar(f1);
    v1->v[1] = BrFloatToScalar(f2);
}

/*
 * v1=-v2
 */
void BR_PUBLIC_ENTRY BrVector2Negate(br_vector2 *v1, br_vector2 *v2)
{
    v1->v[0] = -v2->v[0];
    v1->v[1] = -v2->v[1];
}

/*
 * v1=v2+v3
 */
void BR_PUBLIC_ENTRY BrVector2Add(br_vector2 *v1, br_vector2 *v2, br_vector2 *v3)
{
    v1->v[0] = v2->v[0] + v3->v[0];
    v1->v[1] = v2->v[1] + v3->v[1];
}

/*
 * v1+=v2
 */
void BR_PUBLIC_ENTRY BrVector2Accumulate(br_vector2 *v1, br_vector2 *v2)
{
    v1->v[0] += v2->v[0];
    v1->v[1] += v2->v[1];
}

/*
 * v1=v2-v3
 */
void BR_PUBLIC_ENTRY BrVector2Sub(br_vector2 *v1, br_vector2 *v2, br_vector2 *v3)
{
    v1->v[0] = v2->v[0] - v3->v[0];
    v1->v[1] = v2->v[1] - v3->v[1];
}

/*
 * v1=v2*scalar
 */
void BR_PUBLIC_ENTRY BrVector2Scale(br_vector2 *v1, br_vector2 *v2, br_scalar s)
{
    v1->v[0] = BR_MUL(v2->v[0], (s));
    v1->v[1] = BR_MUL(v2->v[1], (s));
}

/*
 * v1=v2/scalar
 */
void BR_PUBLIC_ENTRY BrVector2InvScale(br_vector2 *v1, br_vector2 *v2, br_scalar s)
{
    v1->v[0] = BR_DIV(v2->v[0], (s));
    v1->v[1] = BR_DIV(v2->v[1], (s));
}

/*
 *  = v1.v2
 */
br_scalar BR_PUBLIC_ENTRY BrVector2Dot(br_vector2 *v1, br_vector2 *v2)
{
    return BR_MAC2(v1->v[0], v2->v[0], v1->v[1], v2->v[1]);
}

/*
 *  = |v1|
 */
br_scalar BR_PUBLIC_ENTRY BrVector2Length(br_vector2 *v1)
{
    return BR_LENGTH2(v1->v[0], v1->v[1]);
}

/*
 *  = |v1| * |v1|
 */
br_scalar BR_PUBLIC_ENTRY BrVector2LengthSquared(br_vector2 *v1)
{
    return BR_SQR2(v1->v[0], v1->v[1]);
}

/**
 ** 3D VECTORS
 **/

/*
 * v1=v2
 */
void BR_PUBLIC_ENTRY BrVector3Copy(br_vector3 *v1, br_vector3 *v2)
{
    v1->v[0] = v2->v[0];
    v1->v[1] = v2->v[1];
    v1->v[2] = v2->v[2];
}

/*
 * v1=(s1,s2,s3)
 */
void BR_PUBLIC_ENTRY BrVector3Set(br_vector3 *v1, br_scalar s1, br_scalar s2, br_scalar s3)
{
    v1->v[0] = s1;
    v1->v[1] = s2;
    v1->v[2] = s3;
}

void BR_PUBLIC_ENTRY BrVector3SetInt(br_vector3 *v1, int i1, int i2, int i3)
{
    v1->v[0] = BrIntToScalar(i1);
    v1->v[1] = BrIntToScalar(i2);
    v1->v[2] = BrIntToScalar(i3);
}

void BR_PUBLIC_ENTRY BrVector3SetFloat(br_vector3 *v1, float f1, float f2, float f3)
{
    v1->v[0] = BrFloatToScalar(f1);
    v1->v[1] = BrFloatToScalar(f2);
    v1->v[2] = BrFloatToScalar(f3);
}

/*
 * v1=-v2
 */
void BR_PUBLIC_ENTRY BrVector3Negate(br_vector3 *v1, br_vector3 *v2)
{
    v1->v[0] = -v2->v[0];
    v1->v[1] = -v2->v[1];
    v1->v[2] = -v2->v[2];
}

/*
 * v1=v2+v3
 */
void BR_PUBLIC_ENTRY BrVector3Add(br_vector3 *v1, br_vector3 *v2, br_vector3 *v3)
{
    v1->v[0] = v2->v[0] + v3->v[0];
    v1->v[1] = v2->v[1] + v3->v[1];
    v1->v[2] = v2->v[2] + v3->v[2];
}

/*
 * v1+=v2
 */
void BR_PUBLIC_ENTRY BrVector3Accumulate(br_vector3 *v1, br_vector3 *v2)
{
    v1->v[0] += v2->v[0];
    v1->v[1] += v2->v[1];
    v1->v[2] += v2->v[2];
}

/*
 * v1=v2-v3
 */
void BR_PUBLIC_ENTRY BrVector3Sub(br_vector3 *v1, br_vector3 *v2, br_vector3 *v3)
{
    v1->v[0] = v2->v[0] - v3->v[0];
    v1->v[1] = v2->v[1] - v3->v[1];
    v1->v[2] = v2->v[2] - v3->v[2];
}

/*
 * v1=v2*scalar
 */
void BR_PUBLIC_ENTRY BrVector3Scale(br_vector3 *v1, br_vector3 *v2, br_scalar s)
{
    v1->v[0] = BR_MUL(v2->v[0], (s));
    v1->v[1] = BR_MUL(v2->v[1], (s));
    v1->v[2] = BR_MUL(v2->v[2], (s));
}

/*
 * v1=v2/scalar
 */
void BR_PUBLIC_ENTRY BrVector3InvScale(br_vector3 *v1, br_vector3 *v2, br_scalar s)
{
    v1->v[0] = BR_DIV(v2->v[0], (s));
    v1->v[1] = BR_DIV(v2->v[1], (s));
    v1->v[2] = BR_DIV(v2->v[2], (s));
}

/*
 *  = v1.v2
 */
br_scalar BR_PUBLIC_ENTRY BrVector3Dot(br_vector3 *v1, br_vector3 *v2)
{
    return BR_MAC3(v1->v[0], v2->v[0], v1->v[1], v2->v[1], v1->v[2], v2->v[2]);
}

/*
 * v1 = v2 x v3
 */
void BR_PUBLIC_ENTRY BrVector3Cross(br_vector3 *v1, br_vector3 *v2, br_vector3 *v3)
{
    v1->v[0] = BR_MUL(v2->v[1], v3->v[2]) - BR_MUL(v2->v[2], v3->v[1]);
    v1->v[1] = BR_MUL(v2->v[2], v3->v[0]) - BR_MUL(v2->v[0], v3->v[2]);
    v1->v[2] = BR_MUL(v2->v[0], v3->v[1]) - BR_MUL(v2->v[1], v3->v[0]);
}

/*
 *  = |v1|
 */
br_scalar BR_PUBLIC_ENTRY BrVector3Length(br_vector3 *v1)
{
    return BR_LENGTH3(v1->v[0], v1->v[1], v1->v[2]);
}

/*
 * = |v1| * |v1|
 */
br_scalar BR_PUBLIC_ENTRY BrVector3LengthSquared(br_vector3 *v1)
{
    return BR_SQR3(v1->v[0], v1->v[1], v1->v[2]);
}

/*
 * v1 = v2/|v2|
 */
void BR_PUBLIC_ENTRY BrVector3Normalise(br_vector3 *v1, br_vector3 *v2)
{
    br_scalar scale;

    scale = BR_LENGTH3(v2->v[0], v2->v[1], v2->v[2]);

    if (scale > BR_SCALAR_EPSILON * 2)
    {

        scale = BR_DIV(BR_SCALAR(1.0), scale);

        v1->v[0] = BR_MUL(v2->v[0], scale);
        v1->v[1] = BR_MUL(v2->v[1], scale);
        v1->v[2] = BR_MUL(v2->v[2], scale);
    }
    else
    {
        v1->v[0] = BR_SCALAR(1.0);
        v1->v[1] = BR_SCALAR(0.0);
        v1->v[2] = BR_SCALAR(0.0);
    }
}

/*
 * v1 = v2/|v2| (low precision)
 */
void BR_PUBLIC_ENTRY BrVector3NormaliseLP(br_vector3 *v1, br_vector3 *v2)
{
    br_scalar scale;

    scale = BR_RLENGTH3(v2->v[0], v2->v[1], v2->v[2]);

    if (scale != BR_SCALAR(0.0))
    {
        v1->v[0] = BR_MUL(v2->v[0], scale);
        v1->v[1] = BR_MUL(v2->v[1], scale);
        v1->v[2] = BR_MUL(v2->v[2], scale);
    }
}

/**
 ** 4D Vectors
 **/
/*
 *  = v1.v2
 */
br_scalar BR_PUBLIC_ENTRY BrVector4Dot(br_vector4 *v1, br_vector4 *v2)
{
    return BR_MAC4(v1->v[0], v2->v[0], v1->v[1], v2->v[1], v1->v[2], v2->v[2], v1->v[3], v2->v[3]);
}

/*
 * v1=v2
 */
void BR_PUBLIC_ENTRY BrVector4Copy(br_vector4 *v1, br_vector4 *v2)
{
    v1->v[0] = v2->v[0];
    v1->v[1] = v2->v[1];
    v1->v[2] = v2->v[2];
    v1->v[3] = v2->v[3];
}

/**
 ** Private ops for fractional vectors
 **/

/*
 *  = v1.v2 (v1 is a unit vector (word components))
 */
br_scalar BrFVector2Dot(br_fvector2 *v1, br_vector2 *v2)
{
    return BR_FMAC2(v1->v[0], v2->v[0], v1->v[1], v2->v[1]);
}

/*
 * v1=v2
 */
void BrFVector3Copy(br_fvector3 *v1, br_vector2 *v2)
{
    v1->v[0] = BrScalarToFraction(v2->v[0]);
    v1->v[1] = BrScalarToFraction(v2->v[1]);
    v1->v[2] = BrScalarToFraction(v2->v[2]);
}

/*
 * v1=v2*scalar
 */
void BrVector3ScaleF(br_vector3 *v1, br_fvector3 *v2, br_scalar s)
{
    v1->v[0] = BR_MUL(BrFractionToScalar(v2->v[0]), s);
    v1->v[1] = BR_MUL(BrFractionToScalar(v2->v[1]), s);
    v1->v[2] = BR_MUL(BrFractionToScalar(v2->v[2]), s);
}

/*
 *  = v1.v2 (v1 is a unit vector (word components))
 */
br_scalar BrFVector3Dot(br_fvector3 *v1, br_vector3 *v2)
{
    return BR_FMAC3(v1->v[0], v2->v[0], v1->v[1], v2->v[1], v1->v[2], v2->v[2]);
}

/*
 * v1 = v2/|v2|
 */
void BrFVector3Normalise(br_fvector3 *v1, br_vector3 *v2)
{
    br_scalar scale;

    scale = BR_LENGTH3(v2->v[0], v2->v[1], v2->v[2]);

    if (scale != BR_SCALAR(0.0))
    {

        scale = BR_DIV(BR_FRACTION(1.0), scale);

        v1->v[0] = (br_fraction)BR_MUL(v2->v[0], scale);
        v1->v[1] = (br_fraction)BR_MUL(v2->v[1], scale);
        v1->v[2] = (br_fraction)BR_MUL(v2->v[2], scale);
    }
    else
    {
        v1->v[0] = BR_FRACTION(1.0);
        v1->v[1] = BR_FRACTION(0.0);
        v1->v[2] = BR_FRACTION(0.0);
    }
}

/*
 * v1 = v2/|v2| (low precision)
 */
void BrFVector3NormaliseLP(br_fvector3 *v1, br_vector3 *v2)
{
    br_scalar scale = BrScalarToFraction(BR_RLENGTH3(v1->v[0], v1->v[1], v1->v[2]));

    v1->v[0] = (br_fraction)BR_MUL(v2->v[0], scale);
    v1->v[1] = (br_fraction)BR_MUL(v2->v[1], scale);
    v1->v[2] = (br_fraction)BR_MUL(v2->v[2], scale);
}

/*
 * v1 = v2/|v2|
 */
void BR_PUBLIC_ENTRY BrVector2Normalise(br_vector2 *v1, br_vector2 *v2)
{
    br_scalar scale;

    scale = BR_LENGTH2(v2->v[0], v2->v[1]);

    if (scale > BR_SCALAR_EPSILON * 2)
    {

        scale = BR_DIV(BR_SCALAR(1.0), scale);

        v1->v[0] = BR_MUL(v2->v[0], scale);
        v1->v[1] = BR_MUL(v2->v[1], scale);
    }
    else
    {
        v1->v[0] = BR_SCALAR(1.0);
        v1->v[1] = BR_SCALAR(0.0);
    }
}
