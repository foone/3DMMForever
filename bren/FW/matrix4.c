/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: matrix4.c 1.2 1995/02/22 21:42:11 sam Exp $
 * $Locker:  $
 *
 * A very unoptimised set of transforms - these should each
 * be re-done for the sparse multiplication.
 *
 * 4x4 Matrices
 *
 */

#include "fw.h"
#include "shortcut.h"

static char rscid[] = "$Id: matrix4.c 1.2 1995/02/22 21:42:11 sam Exp $";

/*
 * A = B
 */
void BR_PUBLIC_ENTRY BrMatrix4Copy(br_matrix4 *A, br_matrix4 *B)
{
    A(0, 0) = B(0, 0);
    A(0, 1) = B(0, 1);
    A(0, 2) = B(0, 2);
    A(0, 3) = B(0, 3);
    A(1, 0) = B(1, 0);
    A(1, 1) = B(1, 1);
    A(1, 2) = B(1, 2);
    A(1, 3) = B(1, 3);
    A(2, 0) = B(2, 0);
    A(2, 1) = B(2, 1);
    A(2, 2) = B(2, 2);
    A(2, 3) = B(2, 3);
    A(3, 0) = B(3, 0);
    A(3, 1) = B(3, 1);
    A(3, 2) = B(3, 2);
    A(3, 3) = B(3, 3);
}

/*
 * A = B*C
 */
void BR_PUBLIC_ENTRY BrMatrix4Mul(br_matrix4 *A, br_matrix4 *B, br_matrix4 *C)
{
    A(0, 0) = BR_MAC4(B(0, 0), C(0, 0), B(0, 1), C(1, 0), B(0, 2), C(2, 0), B(0, 3), C(3, 0));
    A(0, 1) = BR_MAC4(B(0, 0), C(0, 1), B(0, 1), C(1, 1), B(0, 2), C(2, 1), B(0, 3), C(3, 1));
    A(0, 2) = BR_MAC4(B(0, 0), C(0, 2), B(0, 1), C(1, 2), B(0, 2), C(2, 2), B(0, 3), C(3, 2));
    A(0, 3) = BR_MAC4(B(0, 0), C(0, 3), B(0, 1), C(1, 3), B(0, 2), C(2, 3), B(0, 3), C(3, 3));

    A(1, 0) = BR_MAC4(B(1, 0), C(0, 0), B(1, 1), C(1, 0), B(1, 2), C(2, 0), B(1, 3), C(3, 0));
    A(1, 1) = BR_MAC4(B(1, 0), C(0, 1), B(1, 1), C(1, 1), B(1, 2), C(2, 1), B(1, 3), C(3, 1));
    A(1, 2) = BR_MAC4(B(1, 0), C(0, 2), B(1, 1), C(1, 2), B(1, 2), C(2, 2), B(1, 3), C(3, 2));
    A(1, 3) = BR_MAC4(B(1, 0), C(0, 3), B(1, 1), C(1, 3), B(1, 2), C(2, 3), B(1, 3), C(3, 3));

    A(2, 0) = BR_MAC4(B(2, 0), C(0, 0), B(2, 1), C(1, 0), B(2, 2), C(2, 0), B(2, 3), C(3, 0));
    A(2, 1) = BR_MAC4(B(2, 0), C(0, 1), B(2, 1), C(1, 1), B(2, 2), C(2, 1), B(2, 3), C(3, 1));
    A(2, 2) = BR_MAC4(B(2, 0), C(0, 2), B(2, 1), C(1, 2), B(2, 2), C(2, 2), B(2, 3), C(3, 2));
    A(2, 3) = BR_MAC4(B(2, 0), C(0, 3), B(2, 1), C(1, 3), B(2, 2), C(2, 3), B(2, 3), C(3, 3));

    A(3, 0) = BR_MAC4(B(3, 0), C(0, 0), B(3, 1), C(1, 0), B(3, 2), C(2, 0), B(3, 3), C(3, 0));
    A(3, 1) = BR_MAC4(B(3, 0), C(0, 1), B(3, 1), C(1, 1), B(3, 2), C(2, 1), B(3, 3), C(3, 1));
    A(3, 2) = BR_MAC4(B(3, 0), C(0, 2), B(3, 1), C(1, 2), B(3, 2), C(2, 2), B(3, 3), C(3, 2));
    A(3, 3) = BR_MAC4(B(3, 0), C(0, 3), B(3, 1), C(1, 3), B(3, 2), C(2, 3), B(3, 3), C(3, 3));
}

/*
 * mat = Identity
 */
void BR_PUBLIC_ENTRY BrMatrix4Identity(br_matrix4 *mat)
{
    M(0, 0) = S1;
    M(0, 1) = S0;
    M(0, 2) = S0;
    M(0, 3) = S0;
    M(1, 0) = S0;
    M(1, 1) = S1;
    M(1, 2) = S0;
    M(1, 3) = S0;
    M(2, 0) = S0;
    M(2, 1) = S0;
    M(2, 2) = S1;
    M(2, 3) = S0;
    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;
    M(3, 3) = S1;
}

void BR_PUBLIC_ENTRY BrMatrix4Scale(br_matrix4 *mat, br_scalar sx, br_scalar sy, br_scalar sz)
{
    M(0, 0) = sx;
    M(0, 1) = S0;
    M(0, 2) = S0;
    M(0, 3) = S0;
    M(1, 0) = S0;
    M(1, 1) = sy;
    M(1, 2) = S0;
    M(1, 3) = S0;
    M(2, 0) = S0;
    M(2, 1) = S0;
    M(2, 2) = sz;
    M(2, 3) = S0;
    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;
    M(3, 3) = S1;
}

/*
 * Invert a 4x4 matrix:
 *		-1
 * A = B
 *
 * Based on "Matrix Inversion" by Richard Carling - Graphics Gems pg. 470
 *
 * returns the determinant
 */
br_scalar BR_PUBLIC_ENTRY BrMatrix4Inverse(br_matrix4 *A, br_matrix4 *B)
{
    int i, j;
    br_scalar det, idet;

    /* calculate the adjoint matrix */

    BrMatrix4Adjoint(A, B);

    /*
     * Calculate the 4x4 determinant
     *  if the determinant is zero,
     *  then the inverse matrix is not unique.
     */

    det = BrMatrix4Determinant(B);

    if (BR_ABS(det) < BR_SCALAR_EPSILON * 2)
        return S0;

    /*
     * Scale the adjoint matrix to get the inverse
     */
    idet = BR_RCP(det);

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            A(i, j) = BR_MUL(A(i, j), idet);

    return det;
}

#define DETERMINANT2(a, b, c, d) BR_MAC2((a), (d), -(b), (c))

static br_scalar Determinant3(br_scalar a1, br_scalar a2, br_scalar a3, br_scalar b1, br_scalar b2, br_scalar b3,
                              br_scalar c1, br_scalar c2, br_scalar c3)
{
    return BR_MAC3(a1, DETERMINANT2(b2, b3, c2, c3), -b1, DETERMINANT2(a2, a3, c2, c3), c1,
                   DETERMINANT2(a2, a3, b2, b3));
}

/*
 * return determinant(mat)
 *
 */
br_scalar BR_PUBLIC_ENTRY BrMatrix4Determinant(br_matrix4 *mat)
{
    br_scalar a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

    a1 = M(0, 0);
    b1 = M(0, 1);
    c1 = M(0, 2);
    d1 = M(0, 3);

    a2 = M(1, 0);
    b2 = M(1, 1);
    c2 = M(1, 2);
    d2 = M(1, 3);

    a3 = M(2, 0);
    b3 = M(2, 1);
    c3 = M(2, 2);
    d3 = M(2, 3);

    a4 = M(3, 0);
    b4 = M(3, 1);
    c4 = M(3, 2);
    d4 = M(3, 3);

    return BR_MAC4(
        a1, Determinant3(b2, b3, b4, c2, c3, c4, d2, d3, d4), -b1, Determinant3(a2, a3, a4, c2, c3, c4, d2, d3, d4), c1,
        Determinant3(a2, a3, a4, b2, b3, b4, d2, d3, d4), -d1, Determinant3(a2, a3, a4, b2, b3, b4, c2, c3, c4));
}

/*
 * A = adjoint(B)
 */

void BR_PUBLIC_ENTRY BrMatrix4Adjoint(br_matrix4 *A, br_matrix4 *B)
{
    br_scalar a1, a2, a3, a4, b1, b2, b3, b4;
    br_scalar c1, c2, c3, c4, d1, d2, d3, d4;

    a1 = B(0, 0);
    b1 = B(0, 1);
    c1 = B(0, 2);
    d1 = B(0, 3);

    a2 = B(1, 0);
    b2 = B(1, 1);
    c2 = B(1, 2);
    d2 = B(1, 3);

    a3 = B(2, 0);
    b3 = B(2, 1);
    c3 = B(2, 2);
    d3 = B(2, 3);

    a4 = B(3, 0);
    b4 = B(3, 1);
    c4 = B(3, 2);
    d4 = B(3, 3);

    /* row column labeling reversed since we transpose rows & columns */

    A(0, 0) = Determinant3(b2, b3, b4, c2, c3, c4, d2, d3, d4);
    A(1, 0) = -Determinant3(a2, a3, a4, c2, c3, c4, d2, d3, d4);
    A(2, 0) = Determinant3(a2, a3, a4, b2, b3, b4, d2, d3, d4);
    A(3, 0) = -Determinant3(a2, a3, a4, b2, b3, b4, c2, c3, c4);

    A(0, 1) = -Determinant3(b1, b3, b4, c1, c3, c4, d1, d3, d4);
    A(1, 1) = Determinant3(a1, a3, a4, c1, c3, c4, d1, d3, d4);
    A(2, 1) = -Determinant3(a1, a3, a4, b1, b3, b4, d1, d3, d4);
    A(3, 1) = Determinant3(a1, a3, a4, b1, b3, b4, c1, c3, c4);

    A(0, 2) = Determinant3(b1, b2, b4, c1, c2, c4, d1, d2, d4);
    A(1, 2) = -Determinant3(a1, a2, a4, c1, c2, c4, d1, d2, d4);
    A(2, 2) = Determinant3(a1, a2, a4, b1, b2, b4, d1, d2, d4);
    A(3, 2) = -Determinant3(a1, a2, a4, b1, b2, b4, c1, c2, c4);

    A(0, 3) = -Determinant3(b1, b2, b3, c1, c2, c3, d1, d2, d3);
    A(1, 3) = Determinant3(a1, a2, a3, c1, c2, c3, d1, d2, d3);
    A(2, 3) = -Determinant3(a1, a2, a3, b1, b2, b3, d1, d2, d3);
    A(3, 3) = Determinant3(a1, a2, a3, b1, b2, b3, c1, c2, c3);
}

/*
 * Generate a perspective tranform
 *
 */
void BR_PUBLIC_ENTRY BrMatrix4Perspective(br_matrix4 *mat, br_angle field_of_view, br_scalar aspect, br_scalar hither,
                                          br_scalar yon)
{
    br_scalar scale = BR_DIV(BR_COS((br_angle)(field_of_view / 2)), BR_SIN((br_angle)(field_of_view / 2)));

    M(0, 0) = BR_DIV(scale, aspect);
    M(1, 1) = scale;
    M(2, 2) = BR_DIV((yon + hither), (yon - hither));
    M(3, 2) = BR_CONST_MUL(BR_MULDIV(yon, hither, (yon - hither)), -2);

    M(0, 1) = S0;
    M(0, 2) = S0;
    M(0, 3) = S0;
    M(1, 0) = S0;
    M(1, 2) = S0;
    M(1, 3) = S0;
    M(2, 0) = S0;
    M(2, 1) = S0;
    M(2, 3) = BR_SCALAR(-1);
    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 3) = S0;
}

/*
 * vec_a = vec_b * mat
 */
void BR_PUBLIC_ENTRY BrMatrix4Apply(br_vector4 *A, br_vector4 *B, br_matrix4 *C)
{
    A->v[0] = BR_MAC4(B->v[0], C(0, 0), B->v[1], C(1, 0), B->v[2], C(2, 0), B->v[3], C(3, 0));
    A->v[1] = BR_MAC4(B->v[0], C(0, 1), B->v[1], C(1, 1), B->v[2], C(2, 1), B->v[3], C(3, 1));
    A->v[2] = BR_MAC4(B->v[0], C(0, 2), B->v[1], C(1, 2), B->v[2], C(2, 2), B->v[3], C(3, 2));
    A->v[3] = BR_MAC4(B->v[0], C(0, 3), B->v[1], C(1, 3), B->v[2], C(2, 3), B->v[3], C(3, 3));
}

/*
 * [a b c d] = [ e f g 1 ] . M
 */
void BR_PUBLIC_ENTRY BrMatrix4ApplyP(br_vector4 *A, br_vector3 *B, br_matrix4 *C)
{
    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(1, 0), B->v[2], C(2, 0)) + C(3, 0);
    A->v[1] = BR_MAC3(B->v[0], C(0, 1), B->v[1], C(1, 1), B->v[2], C(2, 1)) + C(3, 1);
    A->v[2] = BR_MAC3(B->v[0], C(0, 2), B->v[1], C(1, 2), B->v[2], C(2, 2)) + C(3, 2);
    A->v[3] = BR_MAC3(B->v[0], C(0, 3), B->v[1], C(1, 3), B->v[2], C(2, 3)) + C(3, 3);
}

/*
 * [a b c d] = [ e f g 0 ] . M
 */
void BR_PUBLIC_ENTRY BrMatrix4ApplyV(br_vector4 *A, br_vector3 *B, br_matrix4 *C)
{
    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(1, 0), B->v[2], C(2, 0));
    A->v[1] = BR_MAC3(B->v[0], C(0, 1), B->v[1], C(1, 1), B->v[2], C(2, 1));
    A->v[2] = BR_MAC3(B->v[0], C(0, 2), B->v[1], C(1, 2), B->v[2], C(2, 2));
    A->v[3] = BR_MAC3(B->v[0], C(0, 3), B->v[1], C(1, 3), B->v[2], C(2, 3));
}

/*
 * vec_a = vec_b * mat
 */
void BR_PUBLIC_ENTRY BrMatrix4TApply(br_vector4 *A, br_vector4 *B, br_matrix4 *C)
{
    A->v[0] = BR_MAC4(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2), B->v[3], C(0, 3));
    A->v[1] = BR_MAC4(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2), B->v[3], C(1, 3));
    A->v[2] = BR_MAC4(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2), B->v[3], C(2, 3));
    A->v[3] = BR_MAC4(B->v[0], C(3, 0), B->v[1], C(3, 1), B->v[2], C(3, 2), B->v[3], C(3, 3));
}

/*
 * [a b c d] = [ e f g 1 ] . M
 */
void BR_PUBLIC_ENTRY BrMatrix4TApplyP(br_vector4 *A, br_vector3 *B, br_matrix4 *C)
{
    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2)) + C(0, 3);
    A->v[1] = BR_MAC3(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2)) + C(1, 3);
    A->v[2] = BR_MAC3(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2)) + C(2, 3);
    A->v[3] = BR_MAC3(B->v[0], C(3, 0), B->v[1], C(3, 1), B->v[2], C(3, 2)) + C(3, 3);
}

/*
 * [a b c d] = [ e f g 0 ] . M
 */
void BR_PUBLIC_ENTRY BrMatrix4TApplyV(br_vector4 *A, br_vector3 *B, br_matrix4 *C)
{
    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2));
    A->v[1] = BR_MAC3(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2));
    A->v[2] = BR_MAC3(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2));
    A->v[3] = BR_MAC3(B->v[0], C(3, 0), B->v[1], C(3, 1), B->v[2], C(3, 2));
}

void BR_PUBLIC_ENTRY BrMatrix4Copy34(br_matrix4 *A, br_matrix34 *B)
{
    A(0, 0) = B(0, 0);
    A(0, 1) = B(0, 1);
    A(0, 2) = B(0, 2);
    A(1, 0) = B(1, 0);
    A(1, 1) = B(1, 1);
    A(1, 2) = B(1, 2);
    A(2, 0) = B(2, 0);
    A(2, 1) = B(2, 1);
    A(2, 2) = B(2, 2);
    A(3, 0) = B(3, 0);
    A(3, 1) = B(3, 1);
    A(3, 2) = B(3, 2);
    A(0, 3) = S0;
    A(1, 3) = S0;
    A(2, 3) = S0;
    A(3, 3) = S1;
}

/**
 ** Private functions
 **/

/*
 * A = A*C
 */
void BR_PUBLIC_ENTRY BrMatrix4Pre34(br_matrix4 *A, br_matrix34 *B)
{
    br_matrix4 C = *A;
#define _C(x, y) C.m[x][y]

    A(0, 0) = BR_MAC3(B(0, 0), _C(0, 0), B(0, 1), _C(1, 0), B(0, 2), _C(2, 0));
    A(0, 1) = BR_MAC3(B(0, 0), _C(0, 1), B(0, 1), _C(1, 1), B(0, 2), _C(2, 1));
    A(0, 2) = BR_MAC3(B(0, 0), _C(0, 2), B(0, 1), _C(1, 2), B(0, 2), _C(2, 2));
    A(0, 3) = BR_MAC3(B(0, 0), _C(0, 3), B(0, 1), _C(1, 3), B(0, 2), _C(2, 3));

    A(1, 0) = BR_MAC3(B(1, 0), _C(0, 0), B(1, 1), _C(1, 0), B(1, 2), _C(2, 0));
    A(1, 1) = BR_MAC3(B(1, 0), _C(0, 1), B(1, 1), _C(1, 1), B(1, 2), _C(2, 1));
    A(1, 2) = BR_MAC3(B(1, 0), _C(0, 2), B(1, 1), _C(1, 2), B(1, 2), _C(2, 2));
    A(1, 3) = BR_MAC3(B(1, 0), _C(0, 3), B(1, 1), _C(1, 3), B(1, 2), _C(2, 3));

    A(2, 0) = BR_MAC3(B(2, 0), _C(0, 0), B(2, 1), _C(1, 0), B(2, 2), _C(2, 0));
    A(2, 1) = BR_MAC3(B(2, 0), _C(0, 1), B(2, 1), _C(1, 1), B(2, 2), _C(2, 1));
    A(2, 2) = BR_MAC3(B(2, 0), _C(0, 2), B(2, 1), _C(1, 2), B(2, 2), _C(2, 2));
    A(2, 3) = BR_MAC3(B(2, 0), _C(0, 3), B(2, 1), _C(1, 3), B(2, 2), _C(2, 3));

    A(3, 0) = BR_MAC3(B(3, 0), _C(0, 0), B(3, 1), _C(1, 0), B(3, 2), _C(2, 0)) + _C(3, 0);
    A(3, 1) = BR_MAC3(B(3, 0), _C(0, 1), B(3, 1), _C(1, 1), B(3, 2), _C(2, 1)) + _C(3, 1);
    A(3, 2) = BR_MAC3(B(3, 0), _C(0, 2), B(3, 1), _C(1, 2), B(3, 2), _C(2, 2)) + _C(3, 2);
    A(3, 3) = BR_MAC3(B(3, 0), _C(0, 3), B(3, 1), _C(1, 3), B(3, 2), _C(2, 3)) + _C(3, 3);
}
