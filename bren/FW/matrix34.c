/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: matrix34.c 1.4 1995/03/01 15:26:13 sam Exp $
 * $Locker:  $
 *
 * A very unoptimised set of transforms - these should each
 * be re-done for the sparse multiplication.
 *
 * 3x4 Matrices
 *
 */

#include "fw.h"
#include "shortcut.h"

static char rscid[] = "$Id: matrix34.c 1.4 1995/03/01 15:26:13 sam Exp $";

static br_matrix34 mattmp1, mattmp2;

/*
 * A = B
 */
void BR_PUBLIC_ENTRY BrMatrix34Copy(br_matrix34 *A, br_matrix34 *B)
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
}

/*
 * A = B*C
 */
void BR_PUBLIC_ENTRY BrMatrix34Mul(br_matrix34 *A, br_matrix34 *B, br_matrix34 *C)
{
    A(0, 0) = BR_MAC3(B(0, 0), C(0, 0), B(0, 1), C(1, 0), B(0, 2), C(2, 0));
    A(0, 1) = BR_MAC3(B(0, 0), C(0, 1), B(0, 1), C(1, 1), B(0, 2), C(2, 1));
    A(0, 2) = BR_MAC3(B(0, 0), C(0, 2), B(0, 1), C(1, 2), B(0, 2), C(2, 2));

    A(1, 0) = BR_MAC3(B(1, 0), C(0, 0), B(1, 1), C(1, 0), B(1, 2), C(2, 0));
    A(1, 1) = BR_MAC3(B(1, 0), C(0, 1), B(1, 1), C(1, 1), B(1, 2), C(2, 1));
    A(1, 2) = BR_MAC3(B(1, 0), C(0, 2), B(1, 1), C(1, 2), B(1, 2), C(2, 2));

    A(2, 0) = BR_MAC3(B(2, 0), C(0, 0), B(2, 1), C(1, 0), B(2, 2), C(2, 0));
    A(2, 1) = BR_MAC3(B(2, 0), C(0, 1), B(2, 1), C(1, 1), B(2, 2), C(2, 1));
    A(2, 2) = BR_MAC3(B(2, 0), C(0, 2), B(2, 1), C(1, 2), B(2, 2), C(2, 2));

    A(3, 0) = BR_MAC3(B(3, 0), C(0, 0), B(3, 1), C(1, 0), B(3, 2), C(2, 0)) + C(3, 0);
    A(3, 1) = BR_MAC3(B(3, 0), C(0, 1), B(3, 1), C(1, 1), B(3, 2), C(2, 1)) + C(3, 1);
    A(3, 2) = BR_MAC3(B(3, 0), C(0, 2), B(3, 1), C(1, 2), B(3, 2), C(2, 2)) + C(3, 2);
}

void BR_PUBLIC_ENTRY BrMatrix34Identity(br_matrix34 *mat)
{
    M(0, 0) = S1;
    M(0, 1) = S0;
    M(0, 2) = S0;
    M(1, 0) = S0;
    M(1, 1) = S1;
    M(1, 2) = S0;
    M(2, 0) = S0;
    M(2, 1) = S0;
    M(2, 2) = S1;
    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;
}

/*
 *         Ú                  ¿
 *         ³ 1    0    0    0 ³
 *         ³                  ³
 *         ³ 0  cosé  siné  0 ³
 * Rx(é) = ³                  ³
 *         ³ 0 -siné  cosé  0 ³
 *         ³                  ³
 *         ³ 0    0    0    1 ³
 *         À                  Ù
 */
void BR_PUBLIC_ENTRY BrMatrix34RotateX(br_matrix34 *mat, br_angle rx)
{
    br_scalar s = BR_SIN(rx);
    br_scalar c = BR_COS(rx);

    M(0, 0) = S1;
    M(0, 1) = S0;
    M(0, 2) = S0;
    M(1, 0) = S0;
    M(1, 1) = c;
    M(1, 2) = s;
    M(2, 0) = S0;
    M(2, 1) = -s;
    M(2, 2) = c;
    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;
}

/*
 *         Ú                  ¿
 *         ³ cosé  0 -siné  0 ³
 *         ³                  ³
 *         ³  0    1   0    0 ³
 * Ry(é) = ³                  ³
 *         ³ siné  0  cosé  0 ³
 *         ³                  ³
 *         ³  0    0   0    1 ³
 *         À                  Ù
 */
void BR_PUBLIC_ENTRY BrMatrix34RotateY(br_matrix34 *mat, br_angle ry)
{
    br_scalar s = BR_SIN(ry);
    br_scalar c = BR_COS(ry);

    M(0, 0) = c;
    M(0, 1) = S0;
    M(0, 2) = -s;
    M(1, 0) = S0;
    M(1, 1) = S1;
    M(1, 2) = S0;
    M(2, 0) = s;
    M(2, 1) = S0;
    M(2, 2) = c;
    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;
}

/*
 *         Ú                   ¿
 *         ³  cosé  siné  0  0 ³
 *         ³                   ³
 *         ³ -siné  cosé  0  0 ³
 * Ry(é) = ³                   ³
 *         ³   0     0    1  0 ³
 *         ³                   ³
 *         ³   0     0    0  1 ³
 *         À                   Ù
 */
void BR_PUBLIC_ENTRY BrMatrix34RotateZ(br_matrix34 *mat, br_angle rz)
{
    br_scalar s = BR_SIN(rz);
    br_scalar c = BR_COS(rz);

    M(0, 0) = c;
    M(0, 1) = s;
    M(0, 2) = S0;
    M(1, 0) = -s;
    M(1, 1) = c;
    M(1, 2) = S0;
    M(2, 0) = S0;
    M(2, 1) = S0;
    M(2, 2) = S1;
    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;
}

/*
 * Rotation about an arbitary (normalised) axis
 */
void BR_PUBLIC_ENTRY BrMatrix34Rotate(br_matrix34 *mat, br_angle r, br_vector3 *a)
{
    br_scalar t, s, c;
    br_scalar txy, txz, tyz, sx, sy, sz;

    s = BR_SIN(r);
    c = BR_COS(r);
    t = S1 - c;

    txy = BR_MUL(t, a->v[X]);
    txz = BR_MUL(txy, a->v[Z]);
    txy = BR_MUL(txy, a->v[Y]);
    tyz = BR_MUL(t, BR_MUL(a->v[Y], a->v[Z]));

    sx = BR_MUL(s, a->v[X]);
    sy = BR_MUL(s, a->v[Y]);
    sz = BR_MUL(s, a->v[Z]);

    M(0, 0) = BR_MUL(t, BR_SQR(a->v[X])) + c;
    M(0, 1) = txy + sz;
    M(0, 2) = txz - sy;
    M(1, 0) = txy - sz;
    M(1, 1) = BR_MUL(t, BR_SQR(a->v[Y])) + c;
    M(1, 2) = tyz + sx;
    M(2, 0) = txz + sy;
    M(2, 1) = tyz - sx;
    M(2, 2) = BR_MUL(t, BR_SQR(a->v[Z])) + c;

    M(3, 0) = M(3, 1) = M(3, 2) = S0;
}

/*
 *               Ú                ¿
 *               ³  1   0   0   0 ³
 *               ³                ³
 *               ³  0   1   0   0 ³
 * T(dx,dy,dz) = ³                ³
 *               ³  0   0   1   0 ³
 *               ³                ³
 *               ³  dx  dy  dz  1 ³
 *               À                Ù
 */
void BR_PUBLIC_ENTRY BrMatrix34Translate(br_matrix34 *mat, br_scalar dx, br_scalar dy, br_scalar dz)
{
    M(0, 0) = S1;
    M(0, 1) = S0;
    M(0, 2) = S0;
    M(1, 0) = S0;
    M(1, 1) = S1;
    M(1, 2) = S0;
    M(2, 0) = S0;
    M(2, 1) = S0;
    M(2, 2) = S1;
    M(3, 0) = dx;
    M(3, 1) = dy;
    M(3, 2) = dz;
}

/*
 *               Ú                ¿
 *               ³  sx  0   0   0 ³
 *               ³                ³
 *               ³  0   sy  0   0 ³
 * S(sx,sy,sz) = ³                ³
 *               ³  0   0   sz  0 ³
 *               ³                ³
 *               ³  0   0   0   1 ³
 *               À                Ù
 */
void BR_PUBLIC_ENTRY BrMatrix34Scale(br_matrix34 *mat, br_scalar sx, br_scalar sy, br_scalar sz)
{
    M(0, 0) = sx;
    M(0, 1) = S0;
    M(0, 2) = S0;
    M(1, 0) = S0;
    M(1, 1) = sy;
    M(1, 2) = S0;
    M(2, 0) = S0;
    M(2, 1) = S0;
    M(2, 2) = sz;
    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;
}

/*
 *                 Ú                ¿
 *                 ³  1   sy  sz  0 ³
 *                 ³                ³
 *                 ³  0   1   0   0 ³
 * ShearX(sy,sz) = ³                ³
 *                 ³  0   0   1   0 ³
 *                 ³                ³
 *                 ³  0   0   0   1 ³
 *                 À                Ù
 */
void BR_PUBLIC_ENTRY BrMatrix34ShearX(br_matrix34 *mat, br_scalar sy, br_scalar sz)
{
    M(0, 0) = S1;
    M(0, 1) = sy;
    M(0, 2) = sz;
    M(1, 0) = S0;
    M(1, 1) = S1;
    M(1, 2) = S0;
    M(2, 0) = S0;
    M(2, 1) = S0;
    M(2, 2) = S1;
    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;
}

/*
 *                 Ú                ¿
 *                 ³  1   0   0   0 ³
 *                 ³                ³
 *                 ³  sx  1   sz  0 ³
 * ShearY(sx,sz) = ³                ³
 *                 ³  0   0   1   0 ³
 *                 ³                ³
 *                 ³  0   0   0   1 ³
 *                 À                Ù
 */
void BR_PUBLIC_ENTRY BrMatrix34ShearY(br_matrix34 *mat, br_scalar sx, br_scalar sz)
{
    M(0, 0) = S1;
    M(0, 1) = S0;
    M(0, 2) = S0;
    M(1, 0) = sx;
    M(1, 1) = S1;
    M(1, 2) = sz;
    M(2, 0) = S0;
    M(2, 1) = S0;
    M(2, 2) = S1;
    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;
}

/*
 *                 Ú                ¿
 *                 ³  1   0   0   0 ³
 *                 ³                ³
 *                 ³  0   1   0   0 ³
 * ShearZ(sx,sy) = ³                ³
 *                 ³  sx  sy  1   0 ³
 *                 ³                ³
 *                 ³  0   0   0   1 ³
 *                 À                Ù
 */
void BR_PUBLIC_ENTRY BrMatrix34ShearZ(br_matrix34 *mat, br_scalar sx, br_scalar sy)
{
    M(0, 0) = S1;
    M(0, 1) = S0;
    M(0, 2) = S0;
    M(1, 0) = S0;
    M(1, 1) = S1;
    M(1, 2) = S0;
    M(2, 0) = sx;
    M(2, 1) = sy;
    M(2, 2) = S1;
    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;
}

/*
 * From Graphics Gems II - Fast Matrix Inversion by Kevin Wu (pp. 342)
 *
 * This should be extended to work with flags asociated with each transform.
 *
 *
 * Computes the inverse of a 3D affine matrix; i.e. a matrix with a dimen-
 * sionality of 4 where the right column has the entries (0, 0, 0, 1).
 *
 * This procedure treats the 4 by 4 matrix as a block matrix and
 * calculates the inverse of one submatrix for a significant perform-
 * ance improvement over a general procedure that can invert any non-
 * singular matrix:
 *          --        --          --          --
 *          |          | -1       |    -1      |
 *          | A      0 |          |   A      0 |
 *    -1    |          |          |            |
 *   M   =  |          |     =    |     -1     |
 *          | C      1 |          | -C A     1 |
 *          |          |          |            |
 *          --        --          --          --
 *
 *  where     M is a 4 by 4 matrix,
 *            A is the 3 by 3 ur left submatrix of M,
 *            C is the 1 by 3 lower left submatrix of M.
 *
 * Input:
 *   A   - 3D affine matrix
 *
 * Output:
 *   B  - inverse of 3D affine matrix
 *
 * Returned value:
 *   determinant of matrix
 */
#if 1

br_scalar BR_PUBLIC_ENTRY BrMatrix34Inverse(br_matrix34 *B, br_matrix34 *A)
{
    br_scalar idet, det;
    br_scalar pos, neg, temp;

#define ACCUMULATE                                                                                                     \
    if (temp >= S0)                                                                                                    \
        pos += temp;                                                                                                   \
    else                                                                                                               \
        neg += temp;

#define PRECISION_LIMIT BR_SCALAR(1.0e-15)

#define ABS(a) (((a) < 0) ? -(a) : (a))

    /*
     * Calculate the determinant of submatrix A and determine if the
     * the matrix is singular as limited by the double precision
     * floating-point data representation.
     */
    pos = neg = S0;
    temp = BR_MUL(BR_MUL(A(0, 0), A(1, 1)), A(2, 2));
    ACCUMULATE
    temp = BR_MUL(BR_MUL(A(0, 1), A(1, 2)), A(2, 0));
    ACCUMULATE
    temp = BR_MUL(BR_MUL(A(0, 2), A(1, 0)), A(2, 1));
    ACCUMULATE
    temp = -BR_MUL(BR_MUL(A(0, 2), A(1, 1)), A(2, 0));
    ACCUMULATE
    temp = -BR_MUL(BR_MUL(A(0, 1), A(1, 0)), A(2, 2));
    ACCUMULATE
    temp = -BR_MUL(BR_MUL(A(0, 0), A(1, 2)), A(2, 1));
    ACCUMULATE
    det = pos + neg;

    /*
     * Is the submatrix A singular?
     */
    if (ABS(det) <= BR_SCALAR_EPSILON * 2)
        return S0;

    if ((ABS(BR_DIV(det, (pos - neg))) < PRECISION_LIMIT))
    {
        /*
         * Matrix M has no inverse
         */
        return S0;
    }

    /*
     * Calculate inverse(A) = adj(A) / det(A)
     */
    idet = BR_DIV(S1, det);

    B(0, 0) = BR_MUL(BR_MAC2(A(1, 1), A(2, 2), -A(1, 2), A(2, 1)), idet);
    B(1, 0) = -BR_MUL(BR_MAC2(A(1, 0), A(2, 2), -A(1, 2), A(2, 0)), idet);
    B(2, 0) = BR_MUL(BR_MAC2(A(1, 0), A(2, 1), -A(1, 1), A(2, 0)), idet);
    B(0, 1) = -BR_MUL(BR_MAC2(A(0, 1), A(2, 2), -A(0, 2), A(2, 1)), idet);
    B(1, 1) = BR_MUL(BR_MAC2(A(0, 0), A(2, 2), -A(0, 2), A(2, 0)), idet);
    B(2, 1) = -BR_MUL(BR_MAC2(A(0, 0), A(2, 1), -A(0, 1), A(2, 0)), idet);
    B(0, 2) = BR_MUL(BR_MAC2(A(0, 1), A(1, 2), -A(0, 2), A(1, 1)), idet);
    B(1, 2) = -BR_MUL(BR_MAC2(A(0, 0), A(1, 2), -A(0, 2), A(1, 0)), idet);
    B(2, 2) = BR_MUL(BR_MAC2(A(0, 0), A(1, 1), -A(0, 1), A(1, 0)), idet);

    /*
     * Calculate -C * inverse(A)
     */
    B(3, 0) = -BR_MAC3(A(3, 0), B(0, 0), A(3, 1), B(1, 0), A(3, 2), B(2, 0));
    B(3, 1) = -BR_MAC3(A(3, 0), B(0, 1), A(3, 1), B(1, 1), A(3, 2), B(2, 1));
    B(3, 2) = -BR_MAC3(A(3, 0), B(0, 2), A(3, 1), B(1, 2), A(3, 2), B(2, 2));

    return det;
}
#endif

#if 0

#define AF(x, y) (AF[x][y])
#define BF(x, y) (BF[x][y])

#define BR_FLOAT_EPSILON 1.192092896e-7f

/*
 * Use float intermediates for invertion
 */
br_scalar BR_PUBLIC_ENTRY BrMatrix34Inverse(br_matrix34 *B, br_matrix34 *A)
{
    float	idet,det;
    float	pos, neg, temp;
	float	AF[4][3], BF[4][3];
	int i;

#define ACCUMULATE                                                                                                     \
    if (temp >= 0.0)                                                                                                   \
        pos += temp;                                                                                                   \
    else                                                                                                               \
        neg += temp;
#define PRECISION_LIMIT BR_SCALAR(1.0e-15)
#define ABS(a) (((a) < 0) ? -(a) : (a))

	/*
	 * Convert input to floats
	 */
	for(i=0; i < 4; i++) {
		AF(i,0) = BrScalarToFloat(A(i,0));
		AF(i,1) = BrScalarToFloat(A(i,1));
		AF(i,2) = BrScalarToFloat(A(i,2));
	}

    /*
     * Calculate the determinant of submatrix A and determine if the
     * the matrix is singular as limited by the double precision
     * floating-point data representation.
     */
    pos = neg = 0.0;
    temp =  AF(0,0) * AF(1,1) * AF(2,2);
    ACCUMULATE
    temp =  AF(0,1) * AF(1,2) * AF(2,0);
    ACCUMULATE
    temp =  AF(0,2) * AF(1,0) * AF(2,1);
    ACCUMULATE
    temp = -AF(0,2) * AF(1,1) * AF(2,0);
    ACCUMULATE
    temp = -AF(0,1) * AF(1,0) * AF(2,2);
    ACCUMULATE
    temp = -AF(0,0) * AF(1,2) * AF(2,1);
    ACCUMULATE
    det = pos + neg;

    /*
	 * Is the submatrix A singular?
	 */
    if(ABS(det) <= BR_FLOAT_EPSILON)
		return S0;

	if((ABS(det/(pos - neg)) < PRECISION_LIMIT)) {
        /*
		 * Matrix M has no inverse
		 */
		return S0;
	}

   /*
	* Calculate inverse(A) = adj(A) / det(A)
	*/
    idet = 1.0/det;

    BF(0,0) =  (AF(1,1) * AF(2,2) - AF(1,2) * AF(2,1)) * idet;
    BF(1,0) = -(AF(1,0) * AF(2,2) - AF(1,2) * AF(2,0)) * idet;
    BF(2,0) =  (AF(1,0) * AF(2,1) - AF(1,1) * AF(2,0)) * idet;
    BF(0,1) = -(AF(0,1) * AF(2,2) - AF(0,2) * AF(2,1)) * idet;
    BF(1,1) =  (AF(0,0) * AF(2,2) - AF(0,2) * AF(2,0)) * idet;
    BF(2,1) = -(AF(0,0) * AF(2,1) - AF(0,1) * AF(2,0)) * idet;
    BF(0,2) =  (AF(0,1) * AF(1,2) - AF(0,2) * AF(1,1)) * idet;
    BF(1,2) = -(AF(0,0) * AF(1,2) - AF(0,2) * AF(1,0)) * idet;
    BF(2,2) =  (AF(0,0) * AF(1,1) - AF(0,1) * AF(1,0)) * idet;

    /*
	 * Calculate -C * inverse(A)
	 */
    BF(3,0) = -(AF(3,0) * BF(0,0) + AF(3,1) * BF(1,0) + AF(3,2) * BF(2,0));
    BF(3,1) = -(AF(3,0) * BF(0,1) + AF(3,1) * BF(1,1) + AF(3,2) * BF(2,1));
    BF(3,2) = -(AF(3,0) * BF(0,2) + AF(3,1) * BF(1,2) + AF(3,2) * BF(2,2));

	/*
	 * Convert output back to scalars
	 */
	for(i=0; i < 4; i++) {
		B(i,0) = BrFloatToScalar(BF(i,0));
		B(i,1) = BrFloatToScalar(BF(i,1));
		B(i,2) = BrFloatToScalar(BF(i,2));
	}

    return BrFloatToScalar(det);
}
#endif

/*
 * Invert a length preserving matrix
 *
 * Given:
 *
 *  A     0
 *
 *  C     1
 *
 * Produce -
 *
 *   T
 *  A     0
 *     T
 * -C A	  1
 *
 */
void BR_PUBLIC_ENTRY BrMatrix34LPInverse(br_matrix34 *A, br_matrix34 *B)
{
    A(0, 0) = B(0, 0);
    A(0, 1) = B(1, 0);
    A(0, 2) = B(2, 0);

    A(1, 0) = B(0, 1);
    A(1, 1) = B(1, 1);
    A(1, 2) = B(2, 1);

    A(2, 0) = B(0, 2);
    A(2, 1) = B(1, 2);
    A(2, 2) = B(2, 2);

    /*
     * Calculate -C * inverse(A)
     */
    A(3, 0) = -BR_MAC3(B(3, 0), A(0, 0), B(3, 1), A(1, 0), B(3, 2), A(2, 0));
    A(3, 1) = -BR_MAC3(B(3, 0), A(0, 1), B(3, 1), A(1, 1), B(3, 2), A(2, 1));
    A(3, 2) = -BR_MAC3(B(3, 0), A(0, 2), B(3, 1), A(1, 2), B(3, 2), A(2, 2));
}

void BR_PUBLIC_ENTRY BrMatrix34LPNormalise(br_matrix34 *A, br_matrix34 *B)
{
    BrVector3Normalise((br_vector3 *)A->m[Z], (br_vector3 *)B->m[Z]);
    BrVector3Cross((br_vector3 *)A->m[X], (br_vector3 *)B->m[Y], (br_vector3 *)A->m[Z]);
    BrVector3Normalise((br_vector3 *)A->m[X], (br_vector3 *)A->m[X]);
    BrVector3Cross((br_vector3 *)A->m[Y], (br_vector3 *)A->m[Z], (br_vector3 *)A->m[X]);

    A(3, 0) = B(3, 0);
    A(3, 1) = B(3, 1);
    A(3, 2) = B(3, 2);
}

/*
 * From Graphic Gems II - The Rolling Ball, Andrew J. Hanson (pp. 51)
 *
 * given incremental device scalars, dx,dy, and radius of ball -
 *
 * Find axis of rotation:
 *
 * 	dr = sqrt(dx^2+dy^2)
 * 	nx = -dy/dr
 * 	ny = +dx/dr
 * 	nz = 0
 *
 * Find angle to rotate about n:
 *
 * cos(a) = radius/sqrt(radius^2 + dr^2)
 * sin(a) = dr/sqrt(radius^2 + dr^2)
 *
 * Now plug these values into a rotation by an angle about a vector
 */

void BR_PUBLIC_ENTRY BrMatrix34RollingBall(br_matrix34 *mat, int dx, int dy, int radius)
{
    br_scalar nx, ny;
    br_scalar ca, sa;
    br_scalar dr, h;

    dr = BR_LENGTH2(BrIntToScalar(dx), BrIntToScalar(dy));

    if (dr == BR_SCALAR(0.0))
    {
        BrMatrix34Identity(mat);
        return;
    }

    nx = -BR_DIV(BrIntToScalar(dy), dr);
    ny = BR_DIV(BrIntToScalar(dx), dr);

    h = BR_LENGTH2(BrIntToScalar(radius), dr);
    ca = BR_DIV(BrIntToScalar(radius), h);
    sa = BR_DIV(dr, h);

    M(0, 0) = ca + BR_MUL(BR_MUL(nx, nx), (S1 - ca));
    M(1, 0) = BR_MUL(BR_MUL(nx, ny), (S1 - ca));
    M(2, 0) = BR_MUL(ny, sa);

    M(0, 1) = M(1, 0);
    M(1, 1) = ca + BR_MUL(BR_MUL(ny, ny), (S1 - ca));
    M(2, 1) = -BR_MUL(nx, sa);

    M(0, 2) = -M(2, 0);
    M(1, 2) = -M(2, 1);
    M(2, 2) = ca;

    M(3, 0) = S0;
    M(3, 1) = S0;
    M(3, 2) = S0;
}

void BR_PUBLIC_ENTRY BrMatrix34Copy4(br_matrix34 *A, br_matrix4 *B)
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
}

/*
 * [a b c d] = [ e f g 0 ] . transpose(M)
 * (unit vector)
 *
 * (Private function)
 */
void BrMatrix34TApplyFV(br_vector3 *A, br_fvector3 *B, br_matrix34 *C)
{
    A->v[0] = BR_FMAC3(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2));
    A->v[1] = BR_FMAC3(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2));
    A->v[2] = BR_FMAC3(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2));
}

/*
 * vec_a = vec_b * mat
 */
void BR_PUBLIC_ENTRY BrMatrix34Apply(br_vector3 *A, br_vector4 *B, br_matrix34 *C)
{
    A->v[0] = BR_MAC4(B->v[0], C(0, 0), B->v[1], C(1, 0), B->v[2], C(2, 0), B->v[3], C(3, 0));
    A->v[1] = BR_MAC4(B->v[0], C(0, 1), B->v[1], C(1, 1), B->v[2], C(2, 1), B->v[3], C(3, 1));
    A->v[2] = BR_MAC4(B->v[0], C(0, 2), B->v[1], C(1, 2), B->v[2], C(2, 2), B->v[3], C(3, 2));
}

/*
 * [a b c ] = [ e f g ] . M
 */
void BR_PUBLIC_ENTRY BrMatrix34ApplyP(br_vector3 *A, br_vector3 *B, br_matrix34 *C)
{
    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(1, 0), B->v[2], C(2, 0)) + C(3, 0);
    A->v[1] = BR_MAC3(B->v[0], C(0, 1), B->v[1], C(1, 1), B->v[2], C(2, 1)) + C(3, 1);
    A->v[2] = BR_MAC3(B->v[0], C(0, 2), B->v[1], C(1, 2), B->v[2], C(2, 2)) + C(3, 2);
}

/*
 * [a b c d] = [ e f g 0 ] . M
 */
void BR_PUBLIC_ENTRY BrMatrix34ApplyV(br_vector3 *A, br_vector3 *B, br_matrix34 *C)
{
    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(1, 0), B->v[2], C(2, 0));
    A->v[1] = BR_MAC3(B->v[0], C(0, 1), B->v[1], C(1, 1), B->v[2], C(2, 1));
    A->v[2] = BR_MAC3(B->v[0], C(0, 2), B->v[1], C(1, 2), B->v[2], C(2, 2));
}

/*
 * vec_a = vec_b * transpose(M)
 */
void BR_PUBLIC_ENTRY BrMatrix34TApply(br_vector4 *A, br_vector4 *B, br_matrix34 *C)
{
    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2));
    A->v[1] = BR_MAC3(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2));
    A->v[2] = BR_MAC3(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2));
    A->v[3] = BR_ADD(BR_MAC3(B->v[0], C(3, 0), B->v[1], C(3, 1), B->v[2], C(3, 2)), B->v[3]);
}

/*
 * [a b c] = [ e f g] . transpose(M)
 */
void BR_PUBLIC_ENTRY BrMatrix34TApplyP(br_vector3 *A, br_vector3 *B, br_matrix34 *C)
{
    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2));
    A->v[1] = BR_MAC3(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2));
    A->v[2] = BR_MAC3(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2));
}

/*
 * [a b c d] = [ e f g 0 ] . transpose(M)
 */
void BR_PUBLIC_ENTRY BrMatrix34TApplyV(br_vector3 *A, br_vector3 *B, br_matrix34 *C)
{
    A->v[0] = BR_MAC3(B->v[0], C(0, 0), B->v[1], C(0, 1), B->v[2], C(0, 2));
    A->v[1] = BR_MAC3(B->v[0], C(1, 0), B->v[1], C(1, 1), B->v[2], C(1, 2));
    A->v[2] = BR_MAC3(B->v[0], C(2, 0), B->v[1], C(2, 1), B->v[2], C(2, 2));
}

/*
 * XXX - Apply vectors of vectors through matrix
 *
 * BrMatrix34ApplyNV()
 * BrMatrix34ApplyNP()
 * BrMatrix34ApplyN()
 */

/*
 * Composite matrix operations -
 * pre and post-multiply with an existing matrix
 */

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
