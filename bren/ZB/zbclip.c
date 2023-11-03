/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: zbclip.c 1.4 1995/08/31 16:47:51 sam Exp $
 * $Locker:  $
 *
 * Mesh rendering to produce faces
 */
#include "zb.h"
#include "shortcut.h"
#include "blockops.h"
#include "brassert.h"

static char rscid[] = "$Id: zbclip.c 1.4 1995/08/31 16:47:51 sam Exp $";

#if USER_CLIP
/*
 * Core of 3D homogenous clipper - loosly based on Paul Heckbert's
 * implemetationin Graphics Gems I.
 *
 * Clip a polygon to an arbitary plane eqn.
 */
static int ZbClipFaceToPlane(struct clip_vertex *vp, struct clip_vertex *verts_out, int num_in, br_vector4 *plane,
                             int cmask)
{
    struct clip_vertex *wp = verts_out;
    int num_out = 0;

    struct clip_vertex *up;
    br_scalar t, tu, tv;
    int m;
    br_scalar *usp, *vsp, *wsp;
    /*
     * Go round face, an edge at a time
     */
    up = vp + num_in - 1;

    tu = -BR_MAC4(plane->v[0], up->comp[C_X], plane->v[1], up->comp[C_Y], plane->v[2], up->comp[C_Z], plane->v[3],
                  up->comp[C_W]);

    for (; num_in--; up = vp, tu = tv, vp++)
    {

        tv = -BR_MAC4(plane->v[0], vp->comp[C_X], plane->v[1], vp->comp[C_Y], plane->v[2], vp->comp[C_Z], plane->v[3],
                      vp->comp[C_W]);

        if (tv <= S0)
        {
            /*
             * This vertex is inside clip space
             */
            if (tu <= S0)
            {
                /*
                 * last vertex was as well - add this vertex
                 */
                *wp++ = *vp;
                num_out++;
                continue;
            }

            /*
             * Edge crosses out to in, add intersection	and this vertex
             */
            t = BR_DIVR(tv, (tv - tu));

            usp = up->comp;
            vsp = vp->comp;
            wsp = wp->comp;

            for (m = cmask; m; m >>= 1, usp++, vsp++, wsp++)
                if (m & 1)
                    *wsp = *vsp + BR_MUL(t, (*usp - *vsp));

            wp++;

            /*
             * Copy next vertex
             */
            *wp++ = *vp;
            num_out += 2;
        }
        else
        {
            /*
             * This vertex is outside clip space
             */
            if (tu > S0)
                /*
                 * last vertex was as well - don't do anything
                 */
                continue;

            /*
             * Edge crosses in to out, add intersection
             */

            t = BR_DIVR(tu, (tu - tv));

            usp = up->comp;
            vsp = vp->comp;
            wsp = wp->comp;

            for (m = cmask; m; m >>= 1, usp++, vsp++, wsp++)
                if (m & 1)
                    *wsp = *usp + BR_MUL(t, (*vsp - *usp));

            wp++;
            num_out++;
        }
    }

    return num_out;
}
#endif

#if 0 /* Unused at the moment - has been special cased even further */
/*
 * Special case for plane that has one of the axes as a normal
 */
static int ZbClipFaceToAxisPlane(
		struct clip_vertex *vp,
		struct clip_vertex *verts_out,
		int num_in,
		int sign,
		int axis,
		br_scalar k,
		int cmask)
{
	struct clip_vertex *wp = verts_out;
	int num_out = 0;

	struct clip_vertex *up;
	br_scalar t,tu,tv;
	int m;
	br_scalar *usp,*vsp,*wsp;

	/*
	 * Go round face, an edge at a time
	 */
	up = vp+num_in-1;
	tu = sign*up->comp[axis] - BR_MUL(k,up->comp[C_W]);

	for( ; num_in-- ; up = vp, tu = tv, vp++) {

		tv = sign*vp->comp[axis] - BR_MUL(k,vp->comp[C_W]);

		if(tu <= S0 ^ tv <= S0) {
			/*
			 * Edge crosses plane - add a new vertex
			 */
			t = BR_DIVR(tv,(tv-tu));

			wp->comp[axis] = sign * BR_MUL(k,wp->comp[C_W]);

			usp = up->comp;
			vsp = vp->comp;
			wsp = wp->comp;

			for(m = cmask ; m ; m >>=1, usp++,vsp++,wsp++)
				if(m & 1)
					*wsp = *vsp + BR_MUL(t,(*usp-*vsp));

			wp++;
			num_out++;
		}

		if(tv <= S0) {
			/*
			 * Inside clip space, copy vertex
			 */
			*wp++ = *vp;
			num_out++;
		}
	}

	return num_out;
}
#endif

/*
 * Special case of sign = 1, k = 1.0
 */
static int ZbClipFaceToPlus1(struct clip_vertex *vp, struct clip_vertex *verts_out, int num_in, int axis, int cmask)
{
    struct clip_vertex *wp = verts_out;
    int num_out = 0;

    struct clip_vertex *up;
    br_scalar t, tu, tv;
    int m;
    br_scalar *usp, *vsp, *wsp;

    /*
     * Go round face, an edge at a time
     */
    up = vp + num_in - 1;
    tu = up->comp[axis] - up->comp[C_W];

    for (; num_in--; up = vp, tu = tv, vp++)
    {

        tv = vp->comp[axis] - vp->comp[C_W];

        if (tv <= S0)
        {
            /*
             * This vertex is inside clip space
             */
            if (tu <= S0)
            {
                /*
                 * last vertex was as well - add this vertex
                 */
                *wp++ = *vp;
                num_out++;
                continue;
            }

            /*
             * Edge crosses out to in, add intersection	and this vertex
             */
            t = BR_DIVR(tv, (tv - tu));

            usp = up->comp;
            vsp = vp->comp;
            wsp = wp->comp;

            for (m = cmask; m; m >>= 1, usp++, vsp++, wsp++)
                if (m & 1)
                    *wsp = *vsp + BR_MUL(t, (*usp - *vsp));

            wp->comp[axis] = wp->comp[C_W];

            wp++;

            /*
             * Copy next vertex
             */
            *wp++ = *vp;
            num_out += 2;
        }
        else
        {
            /*
             * This vertex is outside clip space
             */
            if (tu > S0)
                /*
                 * last vertex was as well - don't do anything
                 */
                continue;

            /*
             * Edge crosses in to out, add intersection
             */

            t = BR_DIVR(tu, (tu - tv));

            usp = up->comp;
            vsp = vp->comp;
            wsp = wp->comp;

            for (m = cmask; m; m >>= 1, usp++, vsp++, wsp++)
                if (m & 1)
                    *wsp = *usp + BR_MUL(t, (*vsp - *usp));

            wp->comp[axis] = wp->comp[C_W];

            wp++;
            num_out++;
        }
    }

    return num_out;
}

/*
 * Special case of sign = -1, k = 1.0
 */
static int ZbClipFaceToMinus1(struct clip_vertex *vp, struct clip_vertex *verts_out, int num_in, int axis, int cmask)
{
    struct clip_vertex *wp = verts_out;
    int num_out = 0;

    struct clip_vertex *up;
    br_scalar t, tu, tv;
    int m;
    br_scalar *usp, *vsp, *wsp;

    /*
     * Go round face, an edge at a time
     */
    up = vp + num_in - 1;
    tu = -up->comp[axis] - up->comp[C_W];

    for (; num_in--; up = vp, tu = tv, vp++)
    {

        tv = -vp->comp[axis] - vp->comp[C_W];

        if (tv <= S0)
        {
            /*
             * This vertex is inside clip space
             */
            if (tu <= S0)
            {
                /*
                 * last vertex was as well - add this vertex
                 */
                *wp++ = *vp;
                num_out++;
                continue;
            }
            /*
             * Edge crosses in to out, add intersection	and this vertex
             */
            t = BR_DIVR(tv, (tv - tu));

            usp = up->comp;
            vsp = vp->comp;
            wsp = wp->comp;

            for (m = cmask; m; m >>= 1, usp++, vsp++, wsp++)
                if (m & 1)
                    *wsp = *vsp + BR_MUL(t, (*usp - *vsp));

            wp->comp[axis] = -wp->comp[C_W];

            wp++;

            /*
             * Copy next vertex
             */
            *wp++ = *vp;
            num_out += 2;
        }
        else
        {
            /*
             * This vertex is outside clip space
             */
            if (tu > S0)
                /*
                 * last vertex was as well - don't do anything
                 */
                continue;

            /*
             * Edge crosses in to out, add intersection
             */

            t = BR_DIVR(tu, (tu - tv));

            usp = up->comp;
            vsp = vp->comp;
            wsp = wp->comp;

            for (m = cmask; m; m >>= 1, usp++, vsp++, wsp++)
                if (m & 1)
                    *wsp = *usp + BR_MUL(t, (*vsp - *usp));

            wp->comp[axis] = -wp->comp[C_W];

            wp++;
            num_out++;
        }
    }

    return num_out;
}

/*
 * Special case of sign = 1, k = 0.0
 */
static int ZbClipFaceToZero(struct clip_vertex *vp, struct clip_vertex *verts_out, int num_in, int axis, int cmask)
{
    struct clip_vertex *wp = verts_out;
    int num_out = 0;

    struct clip_vertex *up;
    br_scalar t, tu, tv;
    int m;
    br_scalar *usp, *vsp, *wsp;

    /*
     * Go round face, an edge at a time
     */
    up = vp + num_in - 1;
    tu = up->comp[axis];

    for (; num_in--; up = vp, tu = tv, vp++)
    {

        tv = vp->comp[axis];

        if (tv <= S0)
        {
            /*
             * This vertex is inside clip space
             */
            if (tu <= S0)
            {
                /*
                 * last vertex was as well - add this vertex
                 */
                *wp++ = *vp;
                num_out++;
                continue;
            }
            /*
             * Edge crosses in to out, add intersection	and this vertex
             */
            t = BR_DIVR(tv, (tv - tu));

            usp = up->comp;
            vsp = vp->comp;
            wsp = wp->comp;

            for (m = cmask; m; m >>= 1, usp++, vsp++, wsp++)
                if (m & 1)
                    *wsp = *vsp + BR_MUL(t, (*usp - *vsp));

            wp->comp[axis] = S0;

            wp++;

            /*
             * Copy next vertex
             */
            *wp++ = *vp;
            num_out += 2;
        }
        else
        {
            /*
             * This vertex is outside clip space
             */
            if (tu > S0)
                /*
                 * last vertex was as well - don't do anything
                 */
                continue;

            /*
             * Edge crosses in to out, add intersection
             */

            t = BR_DIVR(tu, (tu - tv));

            usp = up->comp;
            vsp = vp->comp;
            wsp = wp->comp;

            for (m = cmask; m; m >>= 1, usp++, vsp++, wsp++)
                if (m & 1)
                    *wsp = *usp + BR_MUL(t, (*vsp - *usp));

            wp->comp[axis] = S0;

            wp++;
            num_out++;
        }
    }

    return num_out;
}

#if 0

void dprintf(int x, int y, char *fmt,...);

char *component_names[] = {
	"X",
	"Y",
	"Z",
	"W",
	"U",
	"V",
	"I",
	"R",
	"G",
	"B",
	"Q",
};

#endif

/*
 * Clip a a face to the view volume
 */
struct clip_vertex *ZbFaceClip(br_face *fp, struct temp_face *tfp, int mask, int *n_out)
{
    static struct clip_vertex clip_poly_1[16];
    static struct clip_vertex clip_poly_2[16];

    struct clip_vertex *cp_in, *cp_out, *cp_temp;
    struct temp_vertex *tvp;
    int n, i, j, codes, c;

    /*
     * Get face vertices into clip array
     */
    cp_in = clip_poly_1;

    for (i = 0; i < 3; i++, cp_in++)
    {
        tvp = zb.temp_vertices + fp->vertices[i];
        for (j = 0; j < NUM_COMPONENTS; j++)
            cp_in->comp[j] = tvp->comp[j];
    }

#if 0
	dprintf(0,0,"In:");
	for(j=0; j < NUM_COMPONENTS-1; j++) {
		dprintf(7+j*10,0,"%s",component_names[j]);
	}

	cp_temp = clip_poly_1;
	for(i=0; i < 3; i++, cp_temp++) {
		dprintf(0,i+1,"%2d : ");
		for(j=0; j < NUM_COMPONENTS-1; j++) {
			dprintf(7+j*10,i+1,"%08X",cp_temp->comp[j]);
		}
	}
#endif

    n = 3;
    cp_in = clip_poly_1;
    cp_out = clip_poly_2;
    codes = tfp->codes;

    /*
     * Clip against each plane - if necessary
     * After each plane, swap polygon buffers, and quit if
     * polygon is completely clipped away
     *
     * Could table-drive this (outcode,fn,axis,mask)
     */
    if (codes & OUTCODE_HITHER)
    {
        n = ZbClipFaceToZero(cp_in, cp_out, n, Z, mask & ~CM_Z);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

    if (codes & OUTCODE_YON)
    {
        n = ZbClipFaceToMinus1(cp_in, cp_out, n, Z, mask & ~CM_Z);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

    if (codes & OUTCODE_LEFT)
    {
        n = ZbClipFaceToPlus1(cp_in, cp_out, n, X, mask & ~CM_X);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

    if (codes & OUTCODE_RIGHT)
    {
        n = ZbClipFaceToMinus1(cp_in, cp_out, n, X, mask & ~CM_X);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

    if (codes & OUTCODE_TOP)
    {
        n = ZbClipFaceToPlus1(cp_in, cp_out, n, Y, mask & ~CM_Y);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

    if (codes & OUTCODE_BOTTOM)
    {
        n = ZbClipFaceToMinus1(cp_in, cp_out, n, Y, mask & ~CM_Y);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

#if USER_CLIP
    /*
     * User-defined clip plane
     */
    for (c = 0; c < fw.nactive_clip_planes; c++)
    {

        if (!(codes & (OUTCODE_USER << c)))
            continue;

        n = ZbClipFaceToPlane(cp_in, cp_out, n, &fw.active_clip_planes[c].screen_plane, mask);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }
#endif

#if 0
	dprintf(0,10,"Out:");
	for(j=0; j < NUM_COMPONENTS-1; j++) {
		dprintf(7+j*10,10,"%s",component_names[j]);
	}

	cp_temp = cp_in;
	for(i=0; i < n; i++, cp_temp++) {
		dprintf(0,i+11,"%2d : ");
		for(j=0; j < NUM_COMPONENTS-1; j++) {
			dprintf(7+j*10,i+11,"%08X",cp_temp->comp[j]);
		}
	}
#endif

    *n_out = n;
    return cp_in;
}

/*
 * Clip a a face to the view volume
 */
struct clip_vertex *ZbTempClip(struct temp_vertex *tvp, struct temp_face *tfp, int mask, int *n_out)
{
    static struct clip_vertex clip_poly_1[16];
    static struct clip_vertex clip_poly_2[16];

    struct clip_vertex *cp_in, *cp_out, *cp_temp;
    int n, i, j, codes, c;

    /*
     * Get face vertices into clip array
     */
    cp_in = clip_poly_1;

    for (i = 0; i < 3; i++, cp_in++, tvp++)
    {
        for (j = 0; j < NUM_COMPONENTS; j++)
            cp_in->comp[j] = tvp->comp[j];
    }

#if 0
	dprintf(0,0,"In:");
	for(j=0; j < NUM_COMPONENTS-1; j++) {
		dprintf(7+j*10,0,"%s",component_names[j]);
	}

	cp_temp = clip_poly_1;
	for(i=0; i < 3; i++, cp_temp++) {
		dprintf(0,i+1,"%2d : ");
		for(j=0; j < NUM_COMPONENTS-1; j++) {
			dprintf(7+j*10,i+1,"%08X",cp_temp->comp[j]);
		}
	}
#endif

    n = 3;
    cp_in = clip_poly_1;
    cp_out = clip_poly_2;
    codes = tfp->codes;

    /*
     * Clip against each plane - if necessary
     * After each plane, swap polygon buffers, and quit if
     * polygon is completely clipped away
     *
     * Could table-drive this (outcode,fn,axis,mask)
     */
    if (codes & OUTCODE_HITHER)
    {
        n = ZbClipFaceToZero(cp_in, cp_out, n, Z, mask & ~CM_Z);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

    if (codes & OUTCODE_YON)
    {
        n = ZbClipFaceToMinus1(cp_in, cp_out, n, Z, mask & ~CM_Z);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

    if (codes & OUTCODE_LEFT)
    {
        n = ZbClipFaceToPlus1(cp_in, cp_out, n, X, mask & ~CM_X);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

    if (codes & OUTCODE_RIGHT)
    {
        n = ZbClipFaceToMinus1(cp_in, cp_out, n, X, mask & ~CM_X);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

    if (codes & OUTCODE_TOP)
    {
        n = ZbClipFaceToPlus1(cp_in, cp_out, n, Y, mask & ~CM_Y);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

    if (codes & OUTCODE_BOTTOM)
    {
        n = ZbClipFaceToMinus1(cp_in, cp_out, n, Y, mask & ~CM_Y);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }

#if USER_CLIP
    /*
     * User-defined clip plane
     */
    for (c = 0; c < fw.nactive_clip_planes; c++)
    {

        if (!(codes & (OUTCODE_USER << c)))
            continue;

        n = ZbClipFaceToPlane(cp_in, cp_out, n, &fw.active_clip_planes[c].screen_plane, mask);
        if (n < 3)
            return NULL;
        cp_temp = cp_in;
        cp_in = cp_out;
        cp_out = cp_temp;
    }
#endif

#if 0
	dprintf(0,10,"Out:");
	for(j=0; j < NUM_COMPONENTS-1; j++) {
		dprintf(7+j*10,10,"%s",component_names[j]);
	}

	cp_temp = cp_in;
	for(i=0; i < n; i++, cp_temp++) {
		dprintf(0,i+11,"%2d : ");
		for(j=0; j < NUM_COMPONENTS-1; j++) {
			dprintf(7+j*10,i+11,"%08X",cp_temp->comp[j]);
		}
	}
#endif

    *n_out = n;
    return cp_in;
}

