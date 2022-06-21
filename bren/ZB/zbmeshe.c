/*
 * Copyright (c) 1993 Argonaut Software Ltd. All rights reserved.
 *
 * $Id: zbmeshe.c 1.19 1995/08/31 16:47:57 sam Exp $
 * $Locker:  $
 *
 * Mesh rendering to produce points
 */
#include "zb.h"
#include "shortcut.h"
#include "blockops.h"
#include "brassert.h"

static char rscid[] = "$Id: zbmeshe.c 1.19 1995/08/31 16:47:57 sam Exp $";

/*
 * Clip a line to an arbitary plane eqn. Return true if any part
 * of the line remains
 */
static int ClipLineToPlane(struct temp_vertex *in, struct temp_vertex *out, br_vector4 *plane, int cmask)
{
    br_scalar t, tu, tv;
    int m;
    br_scalar *usp, *vsp, *wsp;

    tu = -BR_MAC4(plane->v[0], in[0].comp[C_X], plane->v[1], in[0].comp[C_Y], plane->v[2], in[0].comp[C_Z], plane->v[3],
                  in[0].comp[C_W]);

    tv = -BR_MAC4(plane->v[0], in[1].comp[C_X], plane->v[1], in[1].comp[C_Y], plane->v[2], in[1].comp[C_Z], plane->v[3],
                  in[1].comp[C_W]);

    if (tu <= S0)
    {
        /*
         * First vertex is inside clip space
         */
        out[0] = in[0];

        if (tv <= S0)
        {
            /*
             * last vertex was as well - return whole line
             */
            out[1] = in[1];
            return 1;
        }

        /*
         * Line crosses in to out, truncate to intersection
         */
        t = BR_DIVR(tu, (tu - tv));

        usp = in[0].comp;
        vsp = in[1].comp;
        wsp = out[1].comp;

        for (m = cmask; m; m >>= 1, usp++, vsp++, wsp++)
            if (m & 1)
                *wsp = *usp + BR_MUL(t, (*vsp - *usp));
    }
    else
    {
        /*
         * First vertex is outside clip space
         */
        if (tv > S0)
            /*
             * last vertex was as well - return false
             */
            return 0;

        out[1] = in[1];

        /*
         * Line crosses out to in, truncate to intersection
         */

        t = BR_DIVR(tv, (tv - tu));

        usp = in[0].comp;
        vsp = in[1].comp;
        wsp = out[0].comp;

        for (m = cmask; m; m >>= 1, usp++, vsp++, wsp++)
            if (m & 1)
                *wsp = *vsp + BR_MUL(t, (*usp - *vsp));
    }

    return 1;
}

/*
 * Find out which faces in mesh are towards eye
 *
 * Accumulate visiblity counts for edges and vertices, mark
 * materials, and fill in vertices for edges
 */
static void ZbEdgeFindVisibleFaces(void)
{
    int f, g;
    br_face *fp = zb.model->prepared_faces;
    br_face_group *gp = zb.model->face_groups;
    br_material *group_material;

    for (g = 0; g < zb.model->nface_groups; g++, gp++)
    {
        group_material = gp->material ? gp->material : zb.default_material;

        if (group_material->flags & BR_MATF_TWO_SIDED)
        {
            /*
             * Two sided faces always face the eye
             */
            for (f = 0; f < gp->nfaces; f++, fp++)
            {

                if (!(fp->flags & 1))
                {
                    zb.edge_counts[fp->edges[0]]++;
                    zb.temp_edges[fp->edges[0]].material = group_material;
                    zb.temp_edges[fp->edges[0]].vertices[0] = fp->vertices[0];
                    zb.temp_edges[fp->edges[0]].vertices[1] = fp->vertices[1];
                    zb.vertex_counts[fp->vertices[0]]++;
                    zb.vertex_counts[fp->vertices[1]]++;
                }

                if (!(fp->flags & 2))
                {
                    zb.edge_counts[fp->edges[1]]++;
                    zb.temp_edges[fp->edges[1]].material = group_material;
                    zb.temp_edges[fp->edges[1]].vertices[0] = fp->vertices[1];
                    zb.temp_edges[fp->edges[1]].vertices[1] = fp->vertices[2];
                    zb.vertex_counts[fp->vertices[1]]++;
                    zb.vertex_counts[fp->vertices[2]]++;
                }

                if (!(fp->flags & 4))
                {
                    zb.edge_counts[fp->edges[2]]++;
                    zb.temp_edges[fp->edges[2]].material = group_material;
                    zb.temp_edges[fp->edges[2]].vertices[0] = fp->vertices[2];
                    zb.temp_edges[fp->edges[2]].vertices[1] = fp->vertices[0];
                    zb.vertex_counts[fp->vertices[2]]++;
                    zb.vertex_counts[fp->vertices[0]]++;
                }
            }
            zb.face_group_counts[g] = gp->nfaces;
        }
        else
        {
            /*
             * Check plane eqn. of every face in group against the eye
             */
            zb.face_group_counts[g] = 0;
            for (f = 0; f < gp->nfaces; f++, fp++)
            {

                /*
                 * if Plane_Eqn . Eye <= 0, face is away from eye
                 */
                if (fw.vtos_type == BR_VTOS_PERSPECTIVE)
                {
                    if (BrFVector3Dot(&fp->n, &fw.eye_m) <= fp->d)
                        continue;
                }
                else
                {
                    if (BrFVector3Dot(&fp->n, &fw.eye_m) <= S0)
                        continue;
                }

                if (!(fp->flags & 1))
                {
                    zb.edge_counts[fp->edges[0]]++;
                    zb.temp_edges[fp->edges[0]].material = group_material;
                    zb.temp_edges[fp->edges[0]].vertices[0] = fp->vertices[0];
                    zb.temp_edges[fp->edges[0]].vertices[1] = fp->vertices[1];
                    zb.vertex_counts[fp->vertices[0]]++;
                    zb.vertex_counts[fp->vertices[1]]++;
                }

                if (!(fp->flags & 2))
                {
                    zb.edge_counts[fp->edges[1]]++;
                    zb.temp_edges[fp->edges[1]].material = group_material;
                    zb.temp_edges[fp->edges[1]].vertices[0] = fp->vertices[1];
                    zb.temp_edges[fp->edges[1]].vertices[1] = fp->vertices[2];
                    zb.vertex_counts[fp->vertices[1]]++;
                    zb.vertex_counts[fp->vertices[2]]++;
                }

                if (!(fp->flags & 4))
                {
                    zb.edge_counts[fp->edges[2]]++;
                    zb.temp_edges[fp->edges[2]].material = group_material;
                    zb.temp_edges[fp->edges[2]].vertices[0] = fp->vertices[2];
                    zb.temp_edges[fp->edges[2]].vertices[1] = fp->vertices[0];
                    zb.vertex_counts[fp->vertices[2]]++;
                    zb.vertex_counts[fp->vertices[0]]++;
                }

                zb.face_group_counts[g]++;
            }
        }
    }
}

/*
 * For each edge, check outcodes, fill in edges and allocate temp_vertices
 */
static void ZbEdgeFindEdgesAndVertices(void)
{
    int e, j, v, vt;
    struct temp_edge *tep = zb.temp_edges;
    br_uint_32 combined_codes;

    for (e = 0; e < zb.model->nedges; e++, tep++)
    {

        tep->flag = 0;

        if (zb.edge_counts[e] == 0)
            continue;

        /*
         * Work out AND/OR of outcodes
         */
        combined_codes = zb.temp_vertices[tep->vertices[0]].outcode | zb.temp_vertices[tep->vertices[1]].outcode;
        /*
         * If completely of one edge of view volume (by outcodes)
         *	mark as not visible
         *	continue
         */
        if ((combined_codes & OUTCODES_NOT) != OUTCODES_NOT)
        {
            zb.vertex_counts[tep->vertices[0]]--;
            zb.vertex_counts[tep->vertices[1]]--;
            continue;
        }

        tep->flag = TFF_VISIBLE;

        /*
         * If any outcode is set - mark as needing clipping and remember combined codes
         */
        if (combined_codes & OUTCODES_ALL)
        {
            tep->flag |= TFF_CLIPPED;
            tep->codes = combined_codes & OUTCODES_ALL;
        }
        /*
         * Edge will be rendered
         */
    }
}

#if 0
/*
 * Do per-vertex paramter calculations (intensity, u & v)
 */
static void ZbEdgeFindVertexParameters(void)
{
	int i;
	struct temp_vertex *avp;
	br_vertex *mvp;
	br_material *matp;

	/*
	 * Base vertices
	 */
	avp = zb.temp_vertices;
	mvp = zb.model->prepared_vertices;

	for(i=0; i < zb.model->nprepared_vertices; i++, avp++, mvp++) {
		/*
		 * Ignore if not visible
		 */
		if(zb.vertex_counts[i] == 0)
			continue;

//		matp = zb.vertex_materials[i];

#if 1
		/*
		 * I am not sure this is necessary
		 */
		if(zb.material == NULL)
			continue;
#else
		ASSERT(zb.material != NULL);
#endif

		fw.surface_fn(mvp, &mvp->n, avp->comp);

		if(zb.material->flags & BR_MATF_FORCE_Z_0) {
			avp->v[Z] = 0;
			avp->comp[C_Z] = BR_SCALAR(0.0);
		}
	}
}
#endif

/*
 * Do per-vertex paramter calculations (intensity, u & v)
 */
static void ZbEdgeFindVertexParameters(void)
{
    int gv, g, v;
    struct temp_vertex *avp;
    br_vertex *vp;
    br_vertex_group *gp = zb.model->vertex_groups;

    /*
     * Base vertices
     */
    avp = zb.temp_vertices;
    v = 0;

    for (g = 0; g < zb.model->nvertex_groups; g++, gp++)
    {

        zb.material = gp->material ? gp->material : zb.default_material;

        if ((zb.model->prep_flags & MODUF_VERTEX_GROUPS_MATCH) && (zb.face_group_counts[g] == 0))
        {
            avp += gp->nvertices;
            v += gp->nvertices;
        }
        else
        {
            SurfacePerMaterial(zb.material);

            for (gv = 0, vp = gp->vertices; gv < gp->nvertices; gv++, v++, avp++, vp++)
            {

                /*
                 * Ignore if not visible
                 */
                if (zb.vertex_counts[v] == 0)
                    continue;

                fw.surface_fn(vp, &vp->n, avp->comp);
            }
        }

        /*
         * Handle FORCE_Z_0
         */
        if (zb.material->flags & BR_MATF_FORCE_Z_0)
        {
            avp -= gp->nvertices;
            for (gv = 0; gv < gp->nvertices; gv++, avp++)
            {
                avp->v[Z] = 0;
                avp->comp[C_Z] = BR_SCALAR(0.0);
            }
        }
    }
}

static void ZbLineClipRender(struct temp_edge *tep, int mask, br_line_fn *renderfn, br_uint_32 convert_mask)
{
    static br_vector4 plane_px = BR_VECTOR4(-1, 0, 0, 1);
    static br_vector4 plane_nx = BR_VECTOR4(1, 0, 0, 1);
    static br_vector4 plane_py = BR_VECTOR4(0, -1, 0, 1);
    static br_vector4 plane_ny = BR_VECTOR4(0, 1, 0, 1);
    static br_vector4 plane_pz = BR_VECTOR4(0, 0, -1, 0);
    static br_vector4 plane_nz = BR_VECTOR4(0, 0, 1, 1);

    struct temp_vertex cv0[2], cv1[2];
    struct temp_vertex *cp_in = cv0, *cp_out = cv1, *cp_tmp;
    struct temp_vertex tv[2];
    int i, c;

    cp_in[0] = zb.temp_vertices[tep->vertices[0]];
    cp_in[1] = zb.temp_vertices[tep->vertices[1]];

    if (tep->codes & OUTCODE_LEFT)
    {
        if (ClipLineToPlane(cp_in, cp_out, &plane_px, mask) == 0)
            return;

        cp_tmp = cp_in;
        cp_in = cp_out;
        cp_out = cp_tmp;
    }

    if (tep->codes & OUTCODE_RIGHT)
    {
        if (ClipLineToPlane(cp_in, cp_out, &plane_nx, mask) == 0)
            return;

        cp_tmp = cp_in;
        cp_in = cp_out;
        cp_out = cp_tmp;
    }

    if (tep->codes & OUTCODE_TOP)
    {
        if (ClipLineToPlane(cp_in, cp_out, &plane_py, mask) == 0)
            return;

        cp_tmp = cp_in;
        cp_in = cp_out;
        cp_out = cp_tmp;
    }

    if (tep->codes & OUTCODE_BOTTOM)
    {
        if (ClipLineToPlane(cp_in, cp_out, &plane_ny, mask) == 0)
            return;

        cp_tmp = cp_in;
        cp_in = cp_out;
        cp_out = cp_tmp;
    }

    if (tep->codes & OUTCODE_HITHER)
    {
        if (ClipLineToPlane(cp_in, cp_out, &plane_pz, mask) == 0)
            return;

        cp_tmp = cp_in;
        cp_in = cp_out;
        cp_out = cp_tmp;
    }

    if (tep->codes & OUTCODE_YON)
    {
        if (ClipLineToPlane(cp_in, cp_out, &plane_nz, mask) == 0)
            return;

        cp_tmp = cp_in;
        cp_in = cp_out;
        cp_out = cp_tmp;
    }

#if USER_CLIP
    /*
     * User-defined clip plane
     */
    for (c = 0; c < fw.nactive_clip_planes; c++)
    {

        if (!(tep->codes & (OUTCODE_USER << c)))
            continue;

        if (ClipLineToPlane(cp_in, cp_out, &fw.active_clip_planes[c].screen_plane, mask) == 0)
            return;

        cp_tmp = cp_in;
        cp_in = cp_out;
        cp_out = cp_tmp;
    }
#endif

    /*
     * Re-project vertices
     */
    for (i = 0; i < 2; i++)
    {
        PROJECT_VERTEX(tv + i, cp_in[i].comp[C_X], cp_in[i].comp[C_Y], cp_in[i].comp[C_Z], cp_in[i].comp[C_W]);
        UPDATE_BOUNDS(tv[i]);
#if !BASED_FIXED
        ZbConvertComponents((br_fixed_ls *)tv[i].comp, cp_in[i].comp, convert_mask);
#else
        tv[i].comp[C_W] = cp_in[i].comp[C_W];
        tv[i].comp[C_I] = cp_in[i].comp[C_I];
        tv[i].comp[C_U] = cp_in[i].comp[C_U];
        tv[i].comp[C_V] = cp_in[i].comp[C_V];
        tv[i].comp[C_R] = cp_in[i].comp[C_R];
        tv[i].comp[C_G] = cp_in[i].comp[C_G];
        tv[i].comp[C_B] = cp_in[i].comp[C_B];
#endif
    }

    renderfn((struct temp_vertex_fixed *)(tv + 0), (struct temp_vertex_fixed *)(tv + 1));
}

static void ZbRenderEdges(void)
{
    br_uint_32 e, i;
    struct temp_edge *tep = zb.temp_edges;
    struct zb_material_type *zbmt;
    struct temp_vertex *tvp;
    struct temp_vertex_fixed tv[2];

    for (e = 0; e < zb.model->nedges; e++, tep++)
    {

        switch (tep->flag)
        {

        case TFF_VISIBLE:

            if (tep->material->colour_map)
                zb.texture_buffer = tep->material->colour_map->pixels;

            if (tep->material->index_shade)
                zb.shade_table = tep->material->index_shade->pixels;

            zbmt = tep->material->rptr;

#if !BASED_FIXED
            for (i = 0; i < 2; i++)
            {
                tvp = zb.temp_vertices + tep->vertices[i];
                tv[i].v[X] = tvp->v[X];
                tv[i].v[Y] = tvp->v[Y];
                tv[i].v[Z] = tvp->v[Z];
                ZbConvertComponents(tv[i].comp, tvp->comp, zbmt->convert_mask);
            }
            zbmt->line(tv + 0, tv + 1);
#else
            zbmt->line((struct temp_vertex_fixed *)zb.temp_vertices + tep->vertices[0],
                       (struct temp_vertex_fixed *)zb.temp_vertices + tep->vertices[1]);
#endif
            break;

        case TFF_VISIBLE | TFF_CLIPPED:

            if (tep->material->colour_map)
                zb.texture_buffer = tep->material->colour_map->pixels;

            if (tep->material->index_shade)
                zb.shade_table = tep->material->index_shade->pixels;

            zbmt = tep->material->rptr;
            ZbLineClipRender(tep, zbmt->clip_mask | CM_U | CM_V | CM_I | CM_R | CM_G | CM_B, zbmt->line,
                             zbmt->convert_mask);
            break;
        }
    }
}

/*
 * Render a models points to the screen through the current transform
 */
void ZbMeshRenderEdges(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen)
{
    void *scratch;
    int scratch_size;
    char *sp, *clear_end;
    int inside_out;

    /*
     * Remember model and material in renderer info.
     */
    zb.model = model;
    zb.default_material = material;

    /*
     * Work out amount of scratch space needed for this model
     */

    scratch_size = SCRATCH_ALIGN(model->nedges * sizeof(*zb.temp_edges));
    scratch_size += SCRATCH_ALIGN(model->nedges * sizeof(*zb.edge_counts));
    scratch_size += SCRATCH_ALIGN(model->nface_groups * sizeof(*zb.face_group_counts));
    scratch_size += SCRATCH_ALIGN(model->nprepared_vertices * sizeof(*zb.vertex_counts));
    scratch_size += SCRATCH_ALIGN(model->nprepared_vertices * sizeof(*zb.temp_vertices));

    scratch = BrScratchAllocate(scratch_size + SCRATCH_BOUNDARY);

    /*
     * Allocate scratch areas
     */
    sp = scratch;

    zb.vertex_counts = (void *)sp;
    sp += SCRATCH_ALIGN(model->nprepared_vertices * sizeof(*zb.vertex_counts));

    zb.face_group_counts = (void *)sp;
    sp += SCRATCH_ALIGN(model->nface_groups * sizeof(*zb.face_group_counts));

    zb.edge_counts = (void *)sp;
    sp += SCRATCH_ALIGN(model->nedges * sizeof(*zb.edge_counts));

    clear_end = sp;

    zb.temp_edges = (void *)sp;
    sp += SCRATCH_ALIGN(model->nedges * sizeof(*zb.temp_edges));

    zb.temp_vertices = (void *)sp;

    /*
     * Clear vertex and edge counts
     */
    BrBlockFill(scratch, 0, ((clear_end - (char *)scratch) + 3) / 4);

    /*
     * Build view_to_model
     *
     * Record a flag to say if model is inside out (det of Xfrm is < 0)
     *
     * This should be done in traversal so that special cases can be
     * exploited
     */
    inside_out = BrMatrix34Inverse(&fw.view_to_model, &fw.model_to_view) < S0;

    /*
     * Transform eye point into model space
     */
    BrVector3EyeInModel(&fw.eye_m);

    /*
     * Process lighting for this model
     */
    SurfacePerModel();

    /*
     * Initialise bounds
     */
#if BOUNDING_RECTANGLE_CALL
    zb.bounds[BR_BOUNDS_MIN_X] = zb.bounds[BR_BOUNDS_MIN_Y] = 0x7fffffff;
    zb.bounds[BR_BOUNDS_MAX_X] = zb.bounds[BR_BOUNDS_MAX_Y] = 0x80000001;
#endif

    /*
     * Process this model
     */
    ZbEdgeFindVisibleFaces();
    ZbTransformVertices();
    ZbEdgeFindEdgesAndVertices();
    ZbEdgeFindVertexParameters();

    ZbRenderEdges();

    BrScratchFree(scratch);

#if BOUNDING_RECTANGLE_CALL

    /*
     * Invoke a callback for bounding rectangle
     */
    if (zb.bounds_call)
    {
        br_int_32 int_bounds[4];

        zb.bounds[0] -= BR_SCALAR(0.5);
        zb.bounds[1] -= BR_SCALAR(0.5);
        zb.bounds[2] += BR_SCALAR(0.5);
        zb.bounds[3] += BR_SCALAR(0.5);

        CLAMP_POINT_MIN(zb.bounds[0], zb.bounds[1]);
        CLAMP_POINT_MAX(zb.bounds[2], zb.bounds[3]);

        int_bounds[0] = ScreenToInt(zb.bounds[0]);
        int_bounds[1] = ScreenToInt(zb.bounds[1]) + fw.output->base_y;
        //		int_bounds[2] = ScreenToInt(zb.bounds[2])-1;
        //		int_bounds[3] = ScreenToInt(zb.bounds[3])+fw.output->base_y-1;

        //	Hawkeye - Add 1 pixel to make sure the narrow lines
        //		give a rect. (1 pixel high or wide.)

        int_bounds[2] = ScreenToInt(zb.bounds[2]);
        int_bounds[3] = ScreenToInt(zb.bounds[3]) + fw.output->base_y;

        if ((int_bounds[0] <= int_bounds[2]) && (int_bounds[1] <= int_bounds[3]))
        {
            zb.bounds_call(actor, model, material, NULL, style, &fw.model_to_screen, int_bounds);
        }
    }
#endif
}

