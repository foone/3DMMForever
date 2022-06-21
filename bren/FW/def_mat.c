/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: def_mat.c 1.4 1995/02/22 21:41:31 sam Exp $
 * $Locker:  $
 *
 * default material
 */
#include "brender.h"

static char rscid[] = "$Id: def_mat.c 1.4 1995/02/22 21:41:31 sam Exp $";

/*
 * Default material - this should really be in the renderer structure
 * so that private fields can be per renderer
 */
br_material _BrDefaultMaterial = {
    "default",
    BR_COLOUR_RGB(255, 255, 255), /* colour			*/
    255,                          /* opacity			*/

    BR_UFRACTION(0.10), /* Indexed ka		*/
    BR_UFRACTION(0.70), /*         kd		*/
    BR_UFRACTION(0.0),  /*         ks		*/

    BR_SCALAR(20), /* power			*/
    BR_MATF_LIGHT, /* flags			*/
    {{
        BR_VECTOR2(1, 0), /* map transform	*/
        BR_VECTOR2(0, 1),
        BR_VECTOR2(0, 0),
    }},
    0,
    63, /* index base/range	*/
};
