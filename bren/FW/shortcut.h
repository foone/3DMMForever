/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: shortcut.h 1.5 1995/02/22 21:42:40 sam Exp $
 * $Locker:  $
 *
 * Some shortcut macros to make matrix, euler, and quaternion access
 * shorter to read
 */

/*
 * Matrices
 */
#define M(x, y) mat->m[x][y]
#define A(x, y) A->m[x][y]
#define B(x, y) B->m[x][y]
#define C(x, y) C->m[x][y]

/*
 * Quaternions
 */
#define lX (l->x)
#define lY (l->y)
#define lZ (l->z)
#define lW (l->w)

#define rX (r->x)
#define rY (r->y)
#define rZ (r->z)
#define rW (r->w)

#define qX (q->x)
#define qY (q->y)
#define qZ (q->z)
#define qW (q->w)

#define tX (t.x)
#define tY (t.y)
#define tZ (t.z)
#define tW (t.w)

#define qqX (qq->x)
#define qqY (qq->y)
#define qqZ (qq->z)
#define qqW (qq->w)

#define qi(c) (((br_scalar *)(q))[c])

/*
 * Euler angles
 */
#define eX (euler->a)
#define eY (euler->b)
#define eZ (euler->c)
#define eO (euler->order)

/*
 * Scalars
 */
#define S1 BR_SCALAR(1.0)
#define S0 BR_SCALAR(0.0)

/*
 * For digging around in points etc - makes things slightly easier to read
 */
#define X 0
#define Y 1
#define Z 2
#define W 3

#define U 0
#define V 1
