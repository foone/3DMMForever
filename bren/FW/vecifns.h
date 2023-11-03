/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: vecifns.h 1.9 1995/03/01 15:26:24 sam Exp $
 * $Locker:  $
 *
 * Inline versions of the internal vector operations
 */

#ifndef _VECIFNS_H_
#define _VECIFNS_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * v1=matrix[row]
 */
#define BrVector3CopyMat34Row(v1, matrix, row)                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        (v1)->v[0] = (matrix)->m[(row)][0];                                                                            \
        (v1)->v[1] = (matrix)->m[(row)][1];                                                                            \
        (v1)->v[2] = (matrix)->m[(row)][2];                                                                            \
    } while (0)

/*
 * v1=matrix[row]
 */
#define BrVector3CopyMat34Col(v1, matrix, col)                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        (v1)->v[0] = (matrix)->m[0][(col)];                                                                            \
        (v1)->v[1] = (matrix)->m[1][(col)];                                                                            \
        (v1)->v[2] = (matrix)->m[2][(col)];                                                                            \
    } while (0)

/*
 *  = v1.v2 (v1 is a unit vector (word components))
 */
#define BrFVector2Dot(v1, v2) BR_FMAC2((v1)->v[0], (v2)->v[0], (v1)->v[1], (v2)->v[1])

/*
 * v1=v2
 */
#define BrVector3CopyF(v1, v2)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        (v1)->v[0] = BrFractionToScalar((v2)->v[0]);                                                                   \
        (v1)->v[1] = BrFractionToScalar((v2)->v[1]);                                                                   \
        (v1)->v[2] = BrFractionToScalar((v2)->v[2]);                                                                   \
    } while (0)

#define BrVector3ScaleF(v1, v2, s)                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        (v1)->v[0] = BR_MUL((v2)->v[0], BrFractionToScalar(s));                                                        \
        (v1)->v[1] = BR_MUL((v2)->v[1], BrFractionToScalar(s));                                                        \
        (v1)->v[2] = BR_MUL((v2)->v[2], BrFractionToScalar(s));                                                        \
    } while (0)

#define BrFVector3Scale(v1, v2, s)                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        (v1)->v[0] = BR_MUL(BrFractionToScalar((v2)->v[0]), (s));                                                      \
        (v1)->v[1] = BR_MUL(BrFractionToScalar((v2)->v[1]), (s));                                                      \
        (v1)->v[2] = BR_MUL(BrFractionToScalar((v2)->v[2]), (s));                                                      \
    } while (0)

/*
 * v1=-v2
 */
#define BrFVector3Negate(v1, v2)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        (v1)->v[0] = -(v2)->v[0];                                                                                      \
        (v1)->v[1] = -(v2)->v[1];                                                                                      \
        (v1)->v[2] = -(v2)->v[2];                                                                                      \
    } while (0)

/*
 *  = v1.v2 (v1 is a unit vector (word components))
 */
#define BrFVector3Dot(v1, v2) BR_FMAC3((v1)->v[0], (v2)->v[0], (v1)->v[1], (v2)->v[1], (v1)->v[2], (v2)->v[2])

/*
 * v1 = v2/|v2|
 */
#define BrFVector3Normalise(v1, v2)                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        br_scalar _scale;                                                                                              \
        _scale = BR_LENGTH3((v2)->v[0], (v2)->v[1], (v2)->v[2]);                                                       \
        if (_scale > BR_SCALAR_EPSILON * 2)                                                                            \
        {                                                                                                              \
            _scale = BR_DIV(BR_FRACTION(1.0), _scale);                                                                 \
            (v1)->v[0] = (br_fraction)BR_MUL((v2)->v[0], _scale);                                                      \
            (v1)->v[1] = (br_fraction)BR_MUL((v2)->v[1], _scale);                                                      \
            (v1)->v[2] = (br_fraction)BR_MUL((v2)->v[2], _scale);                                                      \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            (v1)->v[0] = BR_FRACTION(1.0);                                                                             \
            (v1)->v[1] = BR_FRACTION(0.0);                                                                             \
            (v1)->v[2] = BR_FRACTION(0.0);                                                                             \
        }                                                                                                              \
    } while (0)

/*
 * v1 = v2/|v2|
 */
#define BrFVector3NormaliseQuick(v1, v2)                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        scalar _scale;                                                                                                 \
        _scale = BR_DIV(BR_FRACTION(1.0), BR_LENGTH3((v2)->v[0], (v2)->v[1], (v2)->v[2]));                             \
        BrVector3Scale(v1, v2, _scale);                                                                                \
    } while (0)

/*
 * v1 = v2/|v2| (low precision)
 */
#if BASED_FIXED /* XXX - This should not be here */
#define BrFVector3NormaliseLP(v1, v2)                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        br_scalar _scale;                                                                                              \
        _scale = BR_RLENGTH3((v2)->v[0], (v2)->v[1], (v2)->v[2]) >> 1;                                                 \
        BrVector3Scale(v1, v2, _scale);                                                                                \
    } while (0)
#endif

#if BASED_FLOAT
#define BrFVector3NormaliseLP(v1, v2)                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        br_scalar _scale;                                                                                              \
        _scale = BR_RLENGTH3((v2)->v[0], (v2)->v[1], (v2)->v[2]);                                                      \
        BrVector3ScaleF(v1, v2, _scale);                                                                               \
    } while (0)
#endif

/*
 * v1=matrix[row]
 */
#define BrVector4CopyMat4Row(v1, matrix, row)                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        (v1)->v[0] = (matrix)->m[(row)][0];                                                                            \
        (v1)->v[1] = (matrix)->m[(row)][1];                                                                            \
        (v1)->v[2] = (matrix)->m[(row)][2];                                                                            \
        (v1)->v[3] = (matrix)->m[(row)][3];                                                                            \
    } while (0)

/*
 * v1=matrix[column]
 */
#define BrVector4CopyMat4Col(v1, matrix, col)                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        (v1)->v[0] = (matrix)->m[0][(col)];                                                                            \
        (v1)->v[1] = (matrix)->m[1][(col)];                                                                            \
        (v1)->v[2] = (matrix)->m[2][(col)];                                                                            \
        (v1)->v[3] = (matrix)->m[3][(col)];                                                                            \
    } while (0)

#ifdef __cplusplus
};
#endif
#endif
