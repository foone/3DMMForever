/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: zbmeshp.c 1.20 1995/08/31 16:48:00 sam Exp $
 * $Locker:  $
 *
 * Mesh rendering to produce points
 */
#include "zb.h"
#include "shortcut.h"
#include "blockops.h"

static char rscid[] = "$Id: zbmeshp.c 1.20 1995/08/31 16:48:00 sam Exp $";

/*
 * Render visible points
 */
static void ZbPointsRender(void)
{
    int g, v, gv;
    br_vertex_group *gp = zb.model->vertex_groups;
    struct temp_vertex *tvp;
    struct temp_vertex_fixed tv[1];
    struct zb_material_type *zbmt;
    br_material *group_material;

    tvp = zb.temp_vertices;

    for (v = 0, g = 0; g < zb.model->nvertex_groups; g++, gp++)
    {

        if ((zb.model->prep_flags & MODUF_VERTEX_GROUPS_MATCH) && (zb.face_group_counts[g] == 0))
        {
            tvp += gp->nvertices;
            v += gp->nvertices;
            continue;
        }

        zb.material = group_material = (gp->material ? gp->material : zb.default_material);

        if (group_material->colour_map)
            zb.texture_buffer = group_material->colour_map->pixels;

        if (group_material->index_shade)
            zb.shade_table = group_material->index_shade->pixels;

        zbmt = group_material->rptr;

        for (gv = 0; gv < gp->nvertices; gv++, tvp++, v++)
        {

            if ((zb.vertex_counts[v] <= 0) || tvp->outcode != OUTCODES_NOT)
                continue;

#if !BASED_FIXED
            tv[0].v[X] = tvp->v[X];
            tv[0].v[Y] = tvp->v[Y];
            tv[0].v[Z] = tvp->v[Z];
            ZbConvertComponents(tv[0].comp, tvp->comp, zbmt->convert_mask);
            zbmt->point(tv);
#else
            zbmt->point((struct temp_vertex_fixed *)tvp);
#endif
        }
    }
}

/*
 * Do per-vertex parameter calculations (intensity, u & v)
 */
void ZbPointFindVertexParameters(void)
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
        zb.material = (gp->material ? gp->material : zb.default_material);

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

/*
 * Do per face work - find all the faces that are on screen and
 * allocate temp_vertex structures
 */
static void ZbPointFindFacesAndVertices(void)
{
    int f, g, j;
    br_face *fp = zb.model->prepared_faces;
    br_face_group *gp = zb.model->face_groups;
    struct temp_face *tfp = zb.temp_faces;
    br_uint_32 combined_codes;
    br_material *group_material;

    for (g = 0; g < zb.model->nface_groups; g++, gp++)
    {
        /*
         * Skip group if it is not visible
         */
        if (zb.face_group_counts[g] == 0)
        {
            fp += gp->nfaces;
            tfp += gp->nfaces;
            continue;
        }

        group_material = gp->material ? gp->material : zb.default_material;

        for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
        {

            if (tfp->flag == 0)
                continue;

            /*
             * Work out AND/OR of outcodes
             */
            combined_codes = zb.temp_vertices[fp->vertices[0]].outcode | zb.temp_vertices[fp->vertices[1]].outcode |
                             zb.temp_vertices[fp->vertices[2]].outcode;
            /*
             * If completely of one edge of view volume (by outcodes)
             *	mark as not visible
             *	continue
             */
            if ((combined_codes & OUTCODES_NOT) != OUTCODES_NOT)
            {
                zb.face_group_counts[g]--;

                tfp->flag = 0;

                zb.vertex_counts[fp->vertices[0]]--;
                zb.vertex_counts[fp->vertices[1]]--;
                zb.vertex_counts[fp->vertices[2]]--;
                continue;
            }

            /*
             * Face is a least partially visible
             */
        }
    }
}

/*
 * Render a models points to the screen through the current transform
 */
void ZbMeshRenderPoints(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen)
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
    scratch_size = SCRATCH_ALIGN(model->nprepared_faces * sizeof(*zb.temp_faces));
    scratch_size += SCRATCH_ALIGN(model->nface_groups * sizeof(*zb.face_group_counts));
    scratch_size += SCRATCH_ALIGN(model->nprepared_vertices * sizeof(*zb.vertex_counts));
    scratch_size += SCRATCH_ALIGN(model->nprepared_vertices * sizeof(*zb.temp_vertices));

    /*
     * Make sure current scratch buffer is big enough
     */
    scratch = BrScratchAllocate(scratch_size + SCRATCH_BOUNDARY);

    /*
     * Allocate scratch areas
     */
    sp = scratch;

    zb.vertex_counts = (void *)sp;
    sp += SCRATCH_ALIGN(model->nprepared_vertices * sizeof(*zb.vertex_counts));

    zb.face_group_counts = (void *)sp;
    sp += SCRATCH_ALIGN(model->nface_groups * sizeof(*zb.face_group_counts));

    clear_end = sp;

    zb.temp_faces = (void *)sp;
    sp += SCRATCH_ALIGN(model->nprepared_faces * sizeof(*zb.temp_faces));

    zb.temp_vertices = (void *)sp;

    /*
     * Clear vertex counts
     */
    BrBlockFill(scratch, 0, ((clear_end - (char *)scratch) + 3) / 4);

    /*
     * Build view_to_model
     *
     * Record a flag to say if model is inside out (det of Xfrm is < 0)
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
     * Process this model
     */
    /*
     * Initialise bounds
     */
#if BOUNDING_RECTANGLE_CALL
    zb.bounds[BR_BOUNDS_MIN_X] = zb.bounds[BR_BOUNDS_MIN_Y] = 0x7fffffff;
    zb.bounds[BR_BOUNDS_MAX_X] = zb.bounds[BR_BOUNDS_MAX_Y] = 0x80000001;
#endif

    switch (fw.vtos_type)
    {
    case BR_VTOS_PERSPECTIVE:
        ZbFindVisibleFaces();
        break;

    case BR_VTOS_PARALLEL:
        ZbFindVisibleFacesPar();
        break;
    }

    ZbTransformVertices();
    ZbPointFindFacesAndVertices();
    ZbPointFindVertexParameters();

    ZbPointsRender();

    /*
     * Release scratch
     */
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
        int_bounds[2] = ScreenToInt(zb.bounds[2]) - 1;
        int_bounds[3] = ScreenToInt(zb.bounds[3]) + fw.output->base_y - 1;

        if ((int_bounds[0] <= int_bounds[2]) && (int_bounds[1] <= int_bounds[3]))
        {
            zb.bounds_call(actor, model, material, NULL, style, &fw.model_to_screen, int_bounds);
        }
    }
#endif
}
