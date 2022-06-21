/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: def_mdl.c 1.13 1995/03/29 16:40:59 sam Exp $
 * $Locker:  $
 *
 * default model for when an actor is created - a cube of the default
 * material
 */
#include "brender.h"

static char rscid[] = "$Id: def_mdl.c 1.13 1995/03/29 16:40:59 sam Exp $";

/*
 * 8 Vertices
 */
static br_vertex default_model_vertices[] = {
    {BR_VECTOR3(-1.0, -1.0, 1.0), BR_VECTOR2(0.00, 0.99)},  /*    0 */
    {BR_VECTOR3(1.0, -1.0, 1.0), BR_VECTOR2(0.99, 0.99)},   /*    1 */
    {BR_VECTOR3(1.0, 1.0, 1.0), BR_VECTOR2(0.99, 0.99)},    /*    2 */
    {BR_VECTOR3(-1.0, 1.0, 1.0), BR_VECTOR2(0.00, 0.99)},   /*    3 */
    {BR_VECTOR3(-1.0, -1.0, -1.0), BR_VECTOR2(0.00, 0.00)}, /*    4 */
    {BR_VECTOR3(1.0, -1.0, -1.0), BR_VECTOR2(0.99, 0.00)},  /*    5 */
    {BR_VECTOR3(1.0, 1.0, -1.0), BR_VECTOR2(0.99, 0.00)},   /*    6 */
    {BR_VECTOR3(-1.0, 1.0, -1.0), BR_VECTOR2(0.00, 0.00)},  /*    7 */
};

/*
 * 12 Faces
 */
static br_face default_model_faces[] = {
    {{0, 1, 2}, {0}, NULL, 1}, /*    0 */
    {{0, 2, 3}, {0}, NULL, 1}, /*    1 */
    {{0, 4, 5}, {0}, NULL, 1}, /*    2 */
    {{0, 5, 1}, {0}, NULL, 1}, /*    3 */
    {{1, 5, 6}, {0}, NULL, 1}, /*    4 */
    {{1, 6, 2}, {0}, NULL, 1}, /*    5 */
    {{2, 6, 7}, {0}, NULL, 1}, /*    6 */
    {{2, 7, 3}, {0}, NULL, 1}, /*    7 */
    {{3, 7, 4}, {0}, NULL, 1}, /*    8 */
    {{3, 4, 0}, {0}, NULL, 1}, /*    9 */
    {{4, 7, 6}, {0}, NULL, 1}, /*   10 */
    {{4, 6, 5}, {0}, NULL, 1}, /*   11 */
};

br_model _BrDefaultModel = {
    "default_model",           /* identifier	*/
    default_model_vertices,    /* vertices		*/
    default_model_faces,       /* faces		*/
    8,                         /* nvertices	*/
    12,                        /* nfaces		*/
    BR_VECTOR3(0.0, 0.0, 0.0), /* pivot		*/
    BR_MODF_KEEP_ORIGINAL,     /* Flags		*/
};
