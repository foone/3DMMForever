/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: matrixc.c 1.2 1995/02/22 21:42:12 sam Exp $
 * $Locker:  $
 *
 * Composite matrix operations - pre and post-multiply with an existing matrix
 */

#include "fw.h"
#include "shortcut.h"

static char rscid[] = "$Id: matrixc.c 1.2 1995/02/22 21:42:12 sam Exp $";

/**
 ** 3x4 Transforms
 **/

static br_matrix34 mattmp1, mattmp2;

void BR_PUBLIC_ENTRY BrMatrix34Pre(br_matrix34 *mat, br_matrix34 *A)
{
    BrMatrix34Mul(&mattmp2, A, mat);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34Post(br_matrix34 *mat, br_matrix34 *A)
{
    BrMatrix34Mul(&mattmp2, mat, A);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PreRotateX(br_matrix34 *mat, br_angle rx)
{
    BrMatrix34RotateX(&mattmp1, rx);
    BrMatrix34Mul(&mattmp2, &mattmp1, mat);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PostRotateX(br_matrix34 *mat, br_angle rx)
{
    BrMatrix34RotateX(&mattmp1, rx);
    BrMatrix34Mul(&mattmp2, mat, &mattmp1);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PreRotateY(br_matrix34 *mat, br_angle ry)
{
    BrMatrix34RotateY(&mattmp1, ry);
    BrMatrix34Mul(&mattmp2, &mattmp1, mat);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PostRotateY(br_matrix34 *mat, br_angle ry)
{
    BrMatrix34RotateY(&mattmp1, ry);
    BrMatrix34Mul(&mattmp2, mat, &mattmp1);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PreRotateZ(br_matrix34 *mat, br_angle rz)
{
    BrMatrix34RotateZ(&mattmp1, rz);
    BrMatrix34Mul(&mattmp2, &mattmp1, mat);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PostRotateZ(br_matrix34 *mat, br_angle rz)
{
    BrMatrix34RotateZ(&mattmp1, rz);
    BrMatrix34Mul(&mattmp2, mat, &mattmp1);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PreRotate(br_matrix34 *mat, br_angle r, br_vector3 *axis)
{
    BrMatrix34Rotate(&mattmp1, r, axis);
    BrMatrix34Mul(&mattmp2, &mattmp1, mat);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PostRotate(br_matrix34 *mat, br_angle r, br_vector3 *axis)
{
    BrMatrix34Rotate(&mattmp1, r, axis);
    BrMatrix34Mul(&mattmp2, mat, &mattmp1);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PreTranslate(br_matrix34 *mat, br_scalar x, br_scalar y, br_scalar z)
{
    BrMatrix34Translate(&mattmp1, x, y, z);
    BrMatrix34Mul(&mattmp2, &mattmp1, mat);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PostTranslate(br_matrix34 *mat, br_scalar x, br_scalar y, br_scalar z)
{
    BrMatrix34Translate(&mattmp1, x, y, z);
    BrMatrix34Mul(&mattmp2, mat, &mattmp1);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PreScale(br_matrix34 *mat, br_scalar sx, br_scalar sy, br_scalar sz)
{
    BrMatrix34Scale(&mattmp1, sx, sy, sz);
    BrMatrix34Mul(&mattmp2, &mattmp1, mat);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PostScale(br_matrix34 *mat, br_scalar sx, br_scalar sy, br_scalar sz)
{
    BrMatrix34Scale(&mattmp1, sx, sy, sz);
    BrMatrix34Mul(&mattmp2, mat, &mattmp1);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PreShearX(br_matrix34 *mat, br_scalar sy, br_scalar sz)
{
    BrMatrix34ShearX(&mattmp1, sy, sz);
    BrMatrix34Mul(&mattmp2, &mattmp1, mat);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PostShearX(br_matrix34 *mat, br_scalar sy, br_scalar sz)
{
    BrMatrix34ShearX(&mattmp1, sy, sz);
    BrMatrix34Mul(&mattmp2, mat, &mattmp1);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PreShearY(br_matrix34 *mat, br_scalar sx, br_scalar sz)
{
    BrMatrix34ShearY(&mattmp1, sx, sz);
    BrMatrix34Mul(&mattmp2, &mattmp1, mat);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PostShearY(br_matrix34 *mat, br_scalar sx, br_scalar sz)
{
    BrMatrix34ShearY(&mattmp1, sx, sz);
    BrMatrix34Mul(&mattmp2, mat, &mattmp1);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PreShearZ(br_matrix34 *mat, br_scalar sx, br_scalar sy)
{
    BrMatrix34ShearZ(&mattmp1, sx, sy);
    BrMatrix34Mul(&mattmp2, &mattmp1, mat);
    BrMatrix34Copy(mat, &mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix34PostShearZ(br_matrix34 *mat, br_scalar sx, br_scalar sy)
{
    BrMatrix34ShearZ(&mattmp1, sx, sy);
    BrMatrix34Mul(&mattmp2, mat, &mattmp1);
    BrMatrix34Copy(mat, &mattmp2);
}
