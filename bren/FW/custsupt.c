/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: custsupt.c 1.2 1995/02/22 21:41:27 sam Exp $
 * $Locker:  $
 *
 * Support routines for application
 */
#include <string.h>

#include "fw.h"
#include "shortcut.h"
#include "brassert.h"

static char rscid[] = "$Id: custsupt.c 1.2 1995/02/22 21:41:27 sam Exp $";

/*
 * Transform and project 0,0,0 into screen space
 *
 * Return 1 if point is behind eye
 */
br_uint_8 BR_PUBLIC_ENTRY BrOriginToScreenXY(br_vector2 *screen)
{
    int Behind;
    UASSERT(fw.rendering);

    //	Check the Range for possible overflow...

    if (fw.model_to_screen.m[3][3] < BR_SCALAR(1.0))
    {
        screen->v[X] = BR_DIV(fw.vp_width, fw.model_to_screen.m[3][3]);
        screen->v[Y] = BR_DIV(fw.vp_height, fw.model_to_screen.m[3][3]);
        screen->v[X] = BR_MUL(screen->v[X], fw.model_to_screen.m[3][0]);
        screen->v[Y] = BR_MUL(screen->v[Y], fw.model_to_screen.m[3][1]);
        screen->v[X] += fw.vp_ox;
        screen->v[Y] += fw.vp_oy;
    }
    else
    {
        screen->v[X] = fw.vp_ox + BR_MULDIV(fw.vp_width, fw.model_to_screen.m[3][0], fw.model_to_screen.m[3][3]);
        screen->v[Y] = fw.vp_oy + BR_MULDIV(fw.vp_height, fw.model_to_screen.m[3][1], fw.model_to_screen.m[3][3]);
    }
    return (fw.model_to_screen.m[3][2] > BR_SCALAR(0.0));
}

/*
 * Transform and project 0,0,0 into screen space, generate X,Y,Z and return outcode
 *
 * If the point is offscreen, it will not be projected
 */
br_uint_32 BR_PUBLIC_ENTRY BrOriginToScreenXYZO(br_vector3 *screen)
{
    br_uint_32 outcode;

    UASSERT(fw.rendering);

    OUTCODE_POINT(outcode, ((br_vector4 *)(fw.model_to_screen.m[3])));

    if (!(outcode & OUTCODES_ALL))
    {
        screen->v[X] = fw.vp_ox + BR_MULDIV(fw.vp_width, fw.model_to_screen.m[3][0], fw.model_to_screen.m[3][3]);
        screen->v[Y] = fw.vp_oy + BR_MULDIV(fw.vp_height, fw.model_to_screen.m[3][1], fw.model_to_screen.m[3][3]);
        screen->v[Z] = PERSP_DIV_Z(fw.model_to_screen.m[3][2], fw.model_to_screen.m[3][3]);
    }
    return outcode;
}

/*
 * Transform and project a single point into screen space
 *
 * Return 1 if point is behind eye
 */
br_uint_8 BR_PUBLIC_ENTRY BrPointToScreenXY(br_vector2 *screen, br_vector3 *point)
{
    br_vector4 sp;

    UASSERT(fw.rendering);

    BrMatrix4ApplyP(&sp, point, &fw.model_to_screen);

    screen->v[X] = fw.vp_ox + BR_MULDIV(fw.vp_width, sp.v[0], sp.v[3]);
    screen->v[Y] = fw.vp_oy + BR_MULDIV(fw.vp_height, sp.v[1], sp.v[3]);

    return (sp.v[2] > BR_SCALAR(0.0));
}

/*
 * Transform and project a single point into screen space
 *
 * Return 1 if point is behind eye
 */
br_uint_32 BR_PUBLIC_ENTRY BrPointToScreenXYZO(br_vector3 *screen, br_vector3 *point)
{
    br_vector4 sp;
    br_uint_32 outcode;

    UASSERT(fw.rendering);

    BrMatrix4ApplyP(&sp, point, &fw.model_to_screen);

    OUTCODE_POINT(outcode, &sp);

    if (!(outcode & OUTCODES_ALL))
    {
        screen->v[X] = fw.vp_ox + BR_MULDIV(fw.vp_width, sp.v[0], sp.v[3]);
        screen->v[Y] = fw.vp_oy + BR_MULDIV(fw.vp_height, sp.v[1], sp.v[3]);
        screen->v[Z] = PERSP_DIV_Z(sp.v[2], sp.v[3]);
    }
    return outcode;
}

/*
 * Transform and project many points into screen space
 */
void BR_PUBLIC_ENTRY BrPointToScreenXYMany(br_vector2 *screens, br_vector3 *points, br_uint_32 npoints)
{
    br_vector4 sp;
    int i;

    UASSERT(fw.rendering);

    for (i = 0; i < npoints; i++, screens++, points++)
    {
        BrMatrix4ApplyP(&sp, points, &fw.model_to_screen);

        screens->v[X] = fw.vp_ox + BR_MULDIV(fw.vp_width, sp.v[0], sp.v[3]);
        screens->v[Y] = fw.vp_oy + BR_MULDIV(fw.vp_height, sp.v[1], sp.v[3]);
    }
}

/*
 * Transform and project many points into screen space, generate X,Y,Z
 *
 * Outcode each point, and if it is not on screen, don't project it
 */
void BR_PUBLIC_ENTRY BrPointToScreenXYZOMany(br_vector3 *screens, br_uint_32 *outcodes, br_vector3 *points,
                                             br_uint_32 npoints)
{
    br_vector4 sp;
    int i;
    br_uint_32 outcode;

    UASSERT(fw.rendering);

    for (i = 0; i < npoints; i++, screens++, points++, outcodes++)
    {
        BrMatrix4ApplyP(&sp, points, &fw.model_to_screen);

        OUTCODE_POINT(outcode, &sp);
        *outcodes = outcode;

        if (outcode & OUTCODES_ALL)
            continue;

        screens->v[X] = fw.vp_ox + BR_MULDIV(fw.vp_width, sp.v[0], sp.v[3]);
        screens->v[Y] = fw.vp_oy + BR_MULDIV(fw.vp_height, sp.v[1], sp.v[3]);
        screens->v[Z] = PERSP_DIV_Z(sp.v[2], sp.v[3]);
    }
}

#if 0
/*
 * Transform and project many points into screen space, generate X,Y,Z
 *
 */
void BR_PUBLIC_ENTRY BrPointToScreenXYZMany(br_vector3 *screens, br_vector3 *points, br_uint_32 npoints)
{
	br_vector4 sp;
	int i;
	br_uint_32 outcode;

	UASSERT(fw.rendering);

	for(i=0; i< npoints; i++, screens++, points++) {
		BrMatrix4ApplyP(&sp,points,&fw.model_to_screen);

		screens->v[X] = fw.vp_ox + BR_MULDIV(fw.vp_width,sp.v[0],sp.v[3]);
		screens->v[Y] = fw.vp_oy + BR_MULDIV(fw.vp_height,sp.v[1],sp.v[3]);
		screens->v[Z] = PERSP_DIV_Z(sp.v[2],sp.v[3]);
	}
}
#endif
