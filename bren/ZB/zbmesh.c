/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: zbmesh.c 1.67 1995/08/31 16:47:54 sam Exp $
 * $Locker:  $
 *
 * Mesh rendering to produce faces
 */
#include "zb.h"
#include "shortcut.h"
#include "blockops.h"
#include "brassert.h"

static char rscid[] = "$Id: zbmesh.c 1.67 1995/08/31 16:47:54 sam Exp $";

#define SHOW_REVERSED 0

#if BASED_FIXED
#define FAST_PROJECT 1
#define FAST_CULL 1
#endif

#if BASED_FLOAT
#define FAST_PROJECT 0
#define FAST_CULL 0
#endif

static void ClearDirections(void)
{
    int v;

    for (v = 0; v < zb.model->nprepared_vertices; v++)
    {
        zb.temp_vertices[v].direction = 0;
    }

    zb.directions_cleared = 1;
}

/*
 * Find out which faces in mesh are towards eye
 *
 * Accumulate a count of visible faces per group
 */
void ZbFindVisibleFaces(void)
{
    int f, g, n, df;
    br_face *fp = zb.model->prepared_faces;
    br_face_group *gp = zb.model->face_groups;
    struct temp_face *tfp = zb.temp_faces;

    for (g = 0; g < zb.model->nface_groups; g++, gp++)
    {

        /*
         * Record the material to use for each group
         */
        zb.material = (gp->material ? gp->material : zb.default_material);

        if (zb.material->flags & BR_MATF_TWO_SIDED)
        {
            /*
             * Work out which side of face is visible
             */
            if (!zb.directions_cleared)
                ClearDirections();

            for (f = 0, n = 0; f < gp->nfaces; f++, fp++, tfp++)
            {

                tfp->flag = TFF_VISIBLE;
                df = TVDIR_FRONT;

                /*
                 * if Plane_Eqn . Eye <= 0, face is away from eye
                 */
                if (BrFVector3Dot(&fp->n, &fw.eye_m) < fp->d)
                {
                    tfp->flag |= TFF_REVERSED;
                    df = TVDIR_BACK;
                }

                zb.vertex_counts[fp->vertices[0]]++;
                zb.vertex_counts[fp->vertices[1]]++;
                zb.vertex_counts[fp->vertices[2]]++;

                zb.temp_vertices[fp->vertices[0]].direction |= df;
                zb.temp_vertices[fp->vertices[1]].direction |= df;
                zb.temp_vertices[fp->vertices[2]].direction |= df;
            }
            zb.face_group_counts[g] = gp->nfaces;
        }
        else if (zb.material->flags & BR_MATF_ALWAYS_VISIBLE)
        {
            /*
             * Don't check visibility of face
             */
            for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
            {
                tfp->flag = TFF_VISIBLE;
                zb.vertex_counts[fp->vertices[0]]++;
                zb.vertex_counts[fp->vertices[1]]++;
                zb.vertex_counts[fp->vertices[2]]++;
            }
            zb.face_group_counts[g] = gp->nfaces;
        }
        else
        {
            /*
             * Check plane eqn. of every face in group against the eye
             */
            for (f = 0, n = 0; f < gp->nfaces; f++, fp++, tfp++)
            {

                /*
                 * if Plane_Eqn . Eye <= 0, face is away from eye
                 */
                if (BrFVector3Dot(&fp->n, &fw.eye_m) < fp->d)
                {
                    tfp->flag = 0;
                    continue;
                }

                tfp->flag = TFF_VISIBLE;

                zb.vertex_counts[fp->vertices[0]]++;
                zb.vertex_counts[fp->vertices[1]]++;
                zb.vertex_counts[fp->vertices[2]]++;

                n++;
            }
            zb.face_group_counts[g] = n;
        }
    }
}

void ZbFindVisibleFacesPar(void)
{
    int f, g, n, df;
    br_face *fp = zb.model->prepared_faces;
    br_face_group *gp = zb.model->face_groups;
    struct temp_face *tfp = zb.temp_faces;

    for (g = 0; g < zb.model->nface_groups; g++, gp++)
    {

        /*
         * Recored the material to use for each group
         */
        zb.material = (gp->material ? gp->material : zb.default_material);

        if (zb.material->flags & BR_MATF_TWO_SIDED)
        {
            /*
             * Work out which side of face is visible
             */
            if (!zb.directions_cleared)
                ClearDirections();

            for (f = 0, n = 0; f < gp->nfaces; f++, fp++, tfp++)
            {

                tfp->flag = TFF_VISIBLE;
                df = TVDIR_FRONT;

                /*
                 * if Plane_Eqn . Eye <= 0, face is away from eye
                 */
                if (BrFVector3Dot(&fp->n, &fw.eye_m) < S0)
                {
                    tfp->flag |= TFF_REVERSED;
                    df = TVDIR_BACK;
                }

                zb.vertex_counts[fp->vertices[0]]++;
                zb.vertex_counts[fp->vertices[1]]++;
                zb.vertex_counts[fp->vertices[2]]++;

                zb.temp_vertices[fp->vertices[0]].direction |= df;
                zb.temp_vertices[fp->vertices[1]].direction |= df;
                zb.temp_vertices[fp->vertices[2]].direction |= df;
            }
            zb.face_group_counts[g] = gp->nfaces;
        }
        else if (zb.material->flags & BR_MATF_ALWAYS_VISIBLE)
        {

            /*
             * Two sided faces always face the eye
             */
            for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
            {

                tfp->flag = TFF_VISIBLE;

                zb.vertex_counts[fp->vertices[0]]++;
                zb.vertex_counts[fp->vertices[1]]++;
                zb.vertex_counts[fp->vertices[2]]++;
            }
            zb.face_group_counts[g] = gp->nfaces;
        }
        else
        {
            /*
             * Check plane eqn. of every face in group against the eye
             */
            for (f = 0, n = 0; f < gp->nfaces; f++, fp++, tfp++)
            {

                /*
                 * if Plane_Eqn . Eye <= 0, face is away from eye
                 */
                if (BrFVector3Dot(&fp->n, &fw.eye_m) < S0)
                {
                    tfp->flag = 0;
                    continue;
                }

                tfp->flag = TFF_VISIBLE;

                zb.vertex_counts[fp->vertices[0]]++;
                zb.vertex_counts[fp->vertices[1]]++;
                zb.vertex_counts[fp->vertices[2]]++;

                n++;
            }
            zb.face_group_counts[g] = n;
        }
    }
}

/*
 * Transform, project & outcode vertices
 *
 */
void ZbTransformVertices(void)
{
    int i, c;
    struct temp_vertex *tvp;
    br_vector4 screen;

    tvp = zb.temp_vertices;

    for (i = 0; i < zb.model->nprepared_vertices; i++, tvp++)
    {

        /*
         * Ignore if not visible
         */
        if (zb.vertex_counts[i] == 0)
            continue;

        /*
         * Transform into screen space
         */
        BrMatrix4ApplyP(&screen, &zb.model->prepared_vertices[i].p, &fw.model_to_screen);

        tvp->comp[C_X] = screen.v[X];
        tvp->comp[C_Y] = screen.v[Y];
        tvp->comp[C_Z] = screen.v[Z];
        tvp->comp[C_W] = screen.v[W];

        OUTCODE_POINT(tvp->outcode, &screen);

        /*
         * Project if inside clip volume
         */
        if (!(tvp->outcode & OUTCODES_ALL))
        {
            PROJECT_VERTEX(tvp, screen.v[X], screen.v[Y], screen.v[Z], screen.v[W]);
            UPDATE_BOUNDS(*tvp);
        }
    }
}

/*
 * Do per face work - find all the faces that are on screen
 */
void ZbFindFacesAndVertices(void)
{
    int f, g, j;
    br_face *fp = zb.model->prepared_faces;
    br_face_group *gp = zb.model->face_groups;
    struct temp_face *tfp = zb.temp_faces;
    br_uint_32 combined_codes;

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

        zb.material = (gp->material ? gp->material : zb.default_material);

        if (zb.material->prep_flags & MATUF_SURFACE_FACES)
            SurfacePerMaterial(zb.material);

        for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
        {

            if (!tfp->flag)
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
             * If any outcode is set - mark as needing clipping and remember combined codes
             */
            if (combined_codes & OUTCODES_ALL)
            {
                tfp->flag |= TFF_CLIPPED;
                tfp->codes = combined_codes & OUTCODES_ALL;
                zb.face_group_clipped[g] = 1;
            }

            /*
             * Face will be rendered
             */

            /*
             * Do lighting for face if required
             */
            if (zb.material->prep_flags & MATUF_SURFACE_FACES)
                tfp->surface =
                    fw.face_surface_fn(zb.model->prepared_vertices + fp->vertices[0], fp, tfp->flag & TFF_REVERSED);
        }
    }
}

/*
 * Render a clipped face using the indiciated callback for triangle rendering
 */
static void ZbFaceRender(struct clip_vertex *cp_in, int n)
{
    int j;
    struct temp_vertex tv[3], *tvp1, *tvp2, *tvpt;

    /*
     * Render face
     */
    for (j = 0; j < 2; j++, cp_in++)
    {
        PROJECT_VERTEX(tv + j, cp_in->comp[C_X], cp_in->comp[C_Y], cp_in->comp[C_Z], cp_in->comp[C_W]);
        UPDATE_BOUNDS(tv[j]);

#if !BASED_FIXED
        ZbConvertComponents((br_fixed_ls *)tv[j].comp, cp_in->comp, zb.convert_mask);
#else
        tv[j].comp[C_W] = cp_in->comp[C_W];
        tv[j].comp[C_I] = cp_in->comp[C_I];
        tv[j].comp[C_U] = cp_in->comp[C_U];
        tv[j].comp[C_V] = cp_in->comp[C_V];
        tv[j].comp[C_R] = cp_in->comp[C_R];
        tv[j].comp[C_G] = cp_in->comp[C_G];
        tv[j].comp[C_B] = cp_in->comp[C_B];
#endif
    }

    tvp1 = tv + 1;
    tvp2 = tv + 2;

    for (j = 2; j < n; j++, cp_in++)
    {
        PROJECT_VERTEX(tvp2, cp_in->comp[C_X], cp_in->comp[C_Y], cp_in->comp[C_Z], cp_in->comp[C_W]);

        UPDATE_BOUNDS(*tvp2);

#if !BASED_FIXED
        ZbConvertComponents((br_fixed_ls *)tvp2->comp, cp_in->comp, zb.convert_mask);
#else
        tvp2->comp[C_W] = cp_in->comp[C_W];
        tvp2->comp[C_I] = cp_in->comp[C_I];
        tvp2->comp[C_U] = cp_in->comp[C_U];
        tvp2->comp[C_V] = cp_in->comp[C_V];
        tvp2->comp[C_R] = cp_in->comp[C_R];
        tvp2->comp[C_G] = cp_in->comp[C_G];
        tvp2->comp[C_B] = cp_in->comp[C_B];
#endif

        zb.triangle_render((struct temp_vertex_fixed *)tv, (struct temp_vertex_fixed *)tvp1,
                           (struct temp_vertex_fixed *)tvp2);

        tvpt = tvp1;
        tvp1 = tvp2;
        tvp2 = tvpt;
    }
}

/*
 * Render visible faces with clipping
 */
static void ZbRenderFaces(void)
{
    br_face_group *gp = zb.model->face_groups;
    struct temp_face *tfp = zb.temp_faces;
    struct clip_vertex *clipped;
    int g;
    struct zb_material_type *zbmt;

    /*
     * Loop for each group
     */
    for (g = 0; g < zb.model->nface_groups; g++, gp++)
    {

        /*
         * Skip group if it is not visible
         */
        if (zb.face_group_counts[g] == 0)
        {
            tfp += gp->nfaces;
            continue;
        }

        zb.material = (gp->material ? gp->material : zb.default_material);

        /*
         * Extract various bits of info from material type
         */
        ASSERT(zb.material->rptr != NULL);

        zbmt = zb.material->rptr;

        zb.triangle_render = zbmt->triangle;
        zb.clip_mask = zbmt->clip_mask;
#if !BASED_FIXED
        zb.convert_mask = zbmt->convert_mask;
#endif

        if (zb.material->colour_map)
            zb.texture_buffer = zb.material->colour_map->pixels;

        if (zb.material->index_shade)
            zb.shade_table = zb.material->index_shade->pixels;

        /*
         * Call the face group render function
         */
        zbmt->face_group(gp, tfp);
        tfp += gp->nfaces;
    }
}

/*
 * Render a group of faces
 */
void BR_ASM_CALL ZbRenderFaceGroup(br_face_group *gp, struct temp_face *tfp)
{
    br_face *fp;
    struct clip_vertex *clipped;
    int f, g, nclipped;
    struct zb_material_type *zbmt;
    int i;
    struct temp_vertex *tvp, tv[3];
    br_vertex *vp;
    br_fvector3 rev_normal;

    if ((zb.material->flags & BR_MATF_TWO_SIDED) && (zb.material->prep_flags & MATUF_SURFACE_VERTICES))
    {
        SurfacePerMaterial(zb.material);
    }

    /*
     * Go through each face in loop
     */
    for (f = 0, fp = gp->faces; f < gp->nfaces; f++, fp++, tfp++)
    {

        switch (tfp->flag)
        {
            /*
             * Face is not on screen at all
             */
        case 0:
            continue;

            /*
             * Back of face is visible
             */
        case TFF_VISIBLE | TFF_REVERSED:

            if (zb.material->prep_flags & MATUF_SURFACE_VERTICES)
            {

                /*
                 * If any ov the vertices are used in both directions -
                 * relight them with corect normal
                 */
                for (i = 0; i < 3; i++)
                {
                    tvp = zb.temp_vertices + fp->vertices[i];

                    tv[i] = *tvp;
                    if (tv[i].direction == (TVDIR_FRONT | TVDIR_BACK))
                    {
                        vp = zb.model->vertices + fp->vertices[i];

                        BrFVector3Negate(&rev_normal, &vp->n);
                        fw.surface_fn(vp, &rev_normal, tv[i].comp);
                    }
#if !BASED_FIXED
                    ZbConvertComponents((br_fixed_ls *)tv[i].comp, tvp->comp, zb.convert_mask);
#endif
                }

                zb.triangle_render((struct temp_vertex_fixed *)tv + 0, (struct temp_vertex_fixed *)tv + 1,
                                   (struct temp_vertex_fixed *)tv + 2);

                break;
            }

            /* ELSE FALL THROUGH */

            /*
             * Face is fully on screen
             */
        case TFF_VISIBLE:
            /*
             * Render face
             */
#if !BASED_FIXED
            for (i = 0; i < 3; i++)
            {
                tvp = zb.temp_vertices + fp->vertices[i];
                tv[i].v[X] = tvp->v[X];
                tv[i].v[Y] = tvp->v[Y];
                tv[i].v[Z] = tvp->v[Z];

                ZbConvertComponents((br_fixed_ls *)tv[i].comp, tvp->comp, zb.convert_mask);
            }
            zb.triangle_render((struct temp_vertex_fixed *)tv + 0, (struct temp_vertex_fixed *)tv + 1,
                               (struct temp_vertex_fixed *)tv + 2);
#else
            zb.triangle_render((struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[0],
                               (struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[1],
                               (struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[2]);
#endif
            break;

        case TFF_VISIBLE | TFF_CLIPPED | TFF_REVERSED:

            if (zb.material->prep_flags & MATUF_SURFACE_VERTICES)
            {

                /*
                 * If any ov the vertices are used in both directions -
                 * relight them with corect normal
                 */
                for (i = 0; i < 3; i++)
                {
                    tvp = zb.temp_vertices + fp->vertices[i];

                    tv[i] = *tvp;
                    if (tv[i].direction == (TVDIR_FRONT | TVDIR_BACK))
                    {
                        vp = zb.model->vertices + fp->vertices[i];

                        BrFVector3Negate(&rev_normal, &vp->n);
                        fw.surface_fn(vp, &rev_normal, tv[i].comp);
                    }
                }

                if ((clipped = ZbTempClip(tv, tfp, zb.clip_mask, &nclipped)))
                {
                    ZbFaceRender(clipped, nclipped);
                }

                break;
            }

            /* ELSE FALL THROUGH */

            /*
             * Face is partially on screen
             */
        case TFF_VISIBLE | TFF_CLIPPED:

            if ((clipped = ZbFaceClip(fp, tfp, zb.clip_mask, &nclipped)))
            {
                ZbFaceRender(clipped, nclipped);
            }
            break;
        }
    }
}

/*
 * Render a group of faces - Set the current I value from the face
 */
void BR_ASM_CALL ZbRenderFaceGroup_FaceI(br_face_group *gp, struct temp_face *tfp)
{
    br_face *fp;
    struct clip_vertex *clipped;
    int f, g, nclipped;
    struct zb_material_type *zbmt;
    int i;
    struct temp_vertex tv[3], *tvp;
    br_vertex *vp;
    br_fvector3 rev_normal;

    if ((zb.material->flags & BR_MATF_TWO_SIDED) && (zb.material->prep_flags & MATUF_SURFACE_VERTICES))
    {
        SurfacePerMaterial(zb.material);
    }

    /*
     * Go through each face in loop
     */
    for (f = 0, fp = gp->faces; f < gp->nfaces; f++, fp++, tfp++)
    {

        switch (tfp->flag)
        {
            /*
             * Face is not on screen at all
             */
        case 0:
            continue;

        case TFF_VISIBLE | TFF_REVERSED:
            if (zb.material->prep_flags & MATUF_SURFACE_VERTICES)
            {

                zb.pi.current = BrIntToFixed(tfp->surface);

                /*
                 * If any ov the vertices are used in both directions -
                 * relight them with corect normal
                 */
                for (i = 0; i < 3; i++)
                {
                    tvp = zb.temp_vertices + fp->vertices[i];

                    tv[i] = *tvp;
                    if (tv[i].direction == (TVDIR_FRONT | TVDIR_BACK))
                    {
                        vp = zb.model->vertices + fp->vertices[i];

                        BrFVector3Negate(&rev_normal, &vp->n);
                        fw.surface_fn(vp, &rev_normal, tv[i].comp);
                    }
#if !BASED_FIXED
                    ZbConvertComponents((br_fixed_ls *)tv[i].comp, tvp->comp, zb.convert_mask);
#endif
                }

                zb.triangle_render((struct temp_vertex_fixed *)tv + 0, (struct temp_vertex_fixed *)tv + 1,
                                   (struct temp_vertex_fixed *)tv + 2);

                break;
            }

            /* ELSE FALL THROUGH */

            /*
             * Face is fully on screen
             */
        case TFF_VISIBLE:

            /*
             * Render face
             */
            zb.pi.current = BrIntToFixed(tfp->surface);
#if !BASED_FIXED
            for (i = 0; i < 3; i++)
            {
                tvp = zb.temp_vertices + fp->vertices[i];
                tv[i].v[X] = tvp->v[X];
                tv[i].v[Y] = tvp->v[Y];
                tv[i].v[Z] = tvp->v[Z];

                ZbConvertComponents((br_fixed_ls *)tv[i].comp, tvp->comp, zb.convert_mask);
            }
            zb.triangle_render((struct temp_vertex_fixed *)tv + 0, (struct temp_vertex_fixed *)tv + 1,
                               (struct temp_vertex_fixed *)tv + 2);
#else
            zb.triangle_render((struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[0],
                               (struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[1],
                               (struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[2]);
#endif
            break;

        case TFF_VISIBLE | TFF_CLIPPED | TFF_REVERSED:

            if (zb.material->prep_flags & MATUF_SURFACE_VERTICES)
            {

                /*
                 * If any ov the vertices are used in both directions -
                 * relight them with corect normal
                 */
                for (i = 0; i < 3; i++)
                {
                    tvp = zb.temp_vertices + fp->vertices[i];

                    tv[i] = *tvp;
                    if (tv[i].direction == (TVDIR_FRONT | TVDIR_BACK))
                    {
                        vp = zb.model->vertices + fp->vertices[i];

                        BrFVector3Negate(&rev_normal, &vp->n);
                        fw.surface_fn(vp, &rev_normal, tv[i].comp);
                    }
                }

                if ((clipped = ZbTempClip(tv, tfp, zb.clip_mask, &nclipped)))
                {
                    zb.pi.current = BrIntToFixed(tfp->surface);
                    ZbFaceRender(clipped, nclipped);
                }

                break;
            }

            /* ELSE FALL THROUGH */

            /*
             * Face is partially on screen
             */
        case TFF_VISIBLE | TFF_CLIPPED:

            if ((clipped = ZbFaceClip(fp, tfp, zb.clip_mask, &nclipped)))
            {

                zb.pi.current = BrIntToFixed(tfp->surface);

                ZbFaceRender(clipped, nclipped);
            }
            break;
        }
    }
}

/*
 * Render a group of faces - Set the current I value for each
 * vertex from the face
 */
void BR_ASM_CALL ZbRenderFaceGroup_FaceIV(br_face_group *gp, struct temp_face *tfp)
{
    br_face *fp;
    struct clip_vertex *clipped;
    int f, g, nclipped;
    struct zb_material_type *zbmt;
    br_scalar ci;
    int i;
    struct temp_vertex tv[3], *tvp;
    br_vertex *vp;
    br_fvector3 rev_normal;

    /*
     * Go through each face in loop
     */
    for (f = 0, fp = gp->faces; f < gp->nfaces; f++, fp++, tfp++)
    {

        switch (tfp->flag)
        {
            /*
             * Face is not on screen at all
             */
        case 0:
            continue;

        case TFF_VISIBLE | TFF_REVERSED:
            if (zb.material->prep_flags & MATUF_SURFACE_VERTICES)
            {

                ci = BrIntToScalar(tfp->surface);
                zb.temp_vertices[fp->vertices[0]].comp[C_I] = ci;
                zb.temp_vertices[fp->vertices[1]].comp[C_I] = ci;
                zb.temp_vertices[fp->vertices[2]].comp[C_I] = ci;

                /*
                 * If any ov the vertices are used in both directions -
                 * relight them with corect normal
                 */
                for (i = 0; i < 3; i++)
                {
                    tvp = zb.temp_vertices + fp->vertices[i];

                    tv[i] = *tvp;
                    if (tv[i].direction == (TVDIR_FRONT | TVDIR_BACK))
                    {
                        vp = zb.model->vertices + fp->vertices[i];

                        BrFVector3Negate(&rev_normal, &vp->n);
                        fw.surface_fn(vp, &rev_normal, tv[i].comp);
                    }
#if !BASED_FIXED
                    ZbConvertComponents((br_fixed_ls *)tv[i].comp, tvp->comp, zb.convert_mask);
#endif
                }

                zb.triangle_render((struct temp_vertex_fixed *)tv + 0, (struct temp_vertex_fixed *)tv + 1,
                                   (struct temp_vertex_fixed *)tv + 2);

                break;
            }

            /* ELSE FALL THROUGH */

            /*
             * Face is fully on screen
             */
        case TFF_VISIBLE:

            /*
             * Render face
             */
            ci = BrIntToScalar(tfp->surface);
            zb.temp_vertices[fp->vertices[0]].comp[C_I] = ci;
            zb.temp_vertices[fp->vertices[1]].comp[C_I] = ci;
            zb.temp_vertices[fp->vertices[2]].comp[C_I] = ci;

#if !BASED_FIXED
            for (i = 0; i < 3; i++)
            {
                tvp = zb.temp_vertices + fp->vertices[i];
                tv[i].v[X] = tvp->v[X];
                tv[i].v[Y] = tvp->v[Y];
                tv[i].v[Z] = tvp->v[Z];

                ZbConvertComponents((br_fixed_ls *)tv[i].comp, tvp->comp, zb.convert_mask);
            }
            zb.triangle_render((struct temp_vertex_fixed *)tv + 0, (struct temp_vertex_fixed *)tv + 1,
                               (struct temp_vertex_fixed *)tv + 2);
#else
            zb.triangle_render((struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[0],
                               (struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[1],
                               (struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[2]);
#endif
            break;

        case TFF_VISIBLE | TFF_CLIPPED | TFF_REVERSED:

            if (zb.material->prep_flags & MATUF_SURFACE_VERTICES)
            {

                ci = BrIntToScalar(tfp->surface);
                zb.temp_vertices[fp->vertices[0]].comp[C_I] = ci;
                zb.temp_vertices[fp->vertices[1]].comp[C_I] = ci;
                zb.temp_vertices[fp->vertices[2]].comp[C_I] = ci;

                /*
                 * If any ov the vertices are used in both directions -
                 * relight them with corect normal
                 */
                for (i = 0; i < 3; i++)
                {
                    tvp = zb.temp_vertices + fp->vertices[i];

                    tv[i] = *tvp;
                    if (tv[i].direction == (TVDIR_FRONT | TVDIR_BACK))
                    {
                        vp = zb.model->vertices + fp->vertices[i];

                        BrFVector3Negate(&rev_normal, &vp->n);
                        fw.surface_fn(vp, &rev_normal, tv[i].comp);
                    }
                }

                if ((clipped = ZbTempClip(tv, tfp, zb.clip_mask, &nclipped)))
                {
                    ZbFaceRender(clipped, nclipped);
                }

                break;
            }

            /* ELSE FALL THROUGH */

            /*
             * Face is partially on screen
             */
        case TFF_VISIBLE | TFF_CLIPPED:

            ci = BrIntToScalar(tfp->surface);
            zb.temp_vertices[fp->vertices[0]].comp[C_I] = ci;
            zb.temp_vertices[fp->vertices[1]].comp[C_I] = ci;
            zb.temp_vertices[fp->vertices[2]].comp[C_I] = ci;

            if ((clipped = ZbFaceClip(fp, tfp, zb.clip_mask, &nclipped)))
            {

                ZbFaceRender(clipped, nclipped);
            }
            break;
        }
    }
}

/*
 * Render a group of faces - get the current R,G,B values from the face
 */
void BR_ASM_CALL ZbRenderFaceGroup_FaceRGB(br_face_group *gp, struct temp_face *tfp)
{
    br_face *fp;
    struct clip_vertex *clipped;
    int f, g, nclipped;
    struct zb_material_type *zbmt;
    int i;
    struct temp_vertex tv[3], *tvp;
    br_vertex *vp;
    br_fvector3 rev_normal;

    /*
     * Go through each face in loop
     */
    for (f = 0, fp = gp->faces; f < gp->nfaces; f++, fp++, tfp++)
    {

        switch (tfp->flag)
        {
            /*
             * Face is not on screen at all
             */
        case 0:
            continue;

        case TFF_VISIBLE | TFF_REVERSED:
            if (zb.material->prep_flags & MATUF_SURFACE_VERTICES)
            {

                zb.pr.current = BrIntToFixed(BR_RED(tfp->surface));
                zb.pg.current = BrIntToFixed(BR_GRN(tfp->surface));
                zb.pb.current = BrIntToFixed(BR_BLU(tfp->surface));

                /*
                 * If any ov the vertices are used in both directions -
                 * relight them with corect normal
                 */
                for (i = 0; i < 3; i++)
                {
                    tvp = zb.temp_vertices + fp->vertices[i];

                    tv[i] = *tvp;
                    if (tv[i].direction == (TVDIR_FRONT | TVDIR_BACK))
                    {
                        vp = zb.model->vertices + fp->vertices[i];

                        BrFVector3Negate(&rev_normal, &vp->n);
                        fw.surface_fn(vp, &rev_normal, tv[i].comp);
                    }
#if !BASED_FIXED
                    ZbConvertComponents((br_fixed_ls *)tv[i].comp, tvp->comp, zb.convert_mask);
#endif
                }

                zb.triangle_render((struct temp_vertex_fixed *)tv + 0, (struct temp_vertex_fixed *)tv + 1,
                                   (struct temp_vertex_fixed *)tv + 2);

                break;
            }

            /* ELSE FALL THROUGH */

            /*
             * Face is fully on screen
             */
        case TFF_VISIBLE:

            /*
             * Render face
             */
            zb.pr.current = BrIntToFixed(BR_RED(tfp->surface));
            zb.pg.current = BrIntToFixed(BR_GRN(tfp->surface));
            zb.pb.current = BrIntToFixed(BR_BLU(tfp->surface));

#if !BASED_FIXED
            for (i = 0; i < 3; i++)
            {
                tvp = zb.temp_vertices + fp->vertices[i];
                tv[i].v[X] = tvp->v[X];
                tv[i].v[Y] = tvp->v[Y];
                tv[i].v[Z] = tvp->v[Z];

                ZbConvertComponents((br_fixed_ls *)tv[i].comp, tvp->comp, zb.convert_mask);
            }
            zb.triangle_render((struct temp_vertex_fixed *)tv + 0, (struct temp_vertex_fixed *)tv + 1,
                               (struct temp_vertex_fixed *)tv + 2);
#else
            zb.triangle_render((struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[0],
                               (struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[1],
                               (struct temp_vertex_fixed *)zb.temp_vertices + fp->vertices[2]);
#endif
            break;

        case TFF_VISIBLE | TFF_CLIPPED | TFF_REVERSED:

            if (zb.material->prep_flags & MATUF_SURFACE_VERTICES)
            {

                /*
                 * If any ov the vertices are used in both directions -
                 * relight them with corect normal
                 */
                for (i = 0; i < 3; i++)
                {
                    tvp = zb.temp_vertices + fp->vertices[i];

                    tv[i] = *tvp;
                    if (tv[i].direction == (TVDIR_FRONT | TVDIR_BACK))
                    {
                        vp = zb.model->vertices + fp->vertices[i];

                        BrFVector3Negate(&rev_normal, &vp->n);
                        fw.surface_fn(vp, &rev_normal, tv[i].comp);
                    }
                }

                if ((clipped = ZbTempClip(tv, tfp, zb.clip_mask, &nclipped)))
                {
                    zb.pr.current = BrIntToFixed(BR_RED(tfp->surface));
                    zb.pg.current = BrIntToFixed(BR_GRN(tfp->surface));
                    zb.pb.current = BrIntToFixed(BR_BLU(tfp->surface));
                    ZbFaceRender(clipped, nclipped);
                }

                break;
            }

            /* ELSE FALL THROUGH */

            /*
             * Face is partially on screen
             */
        case TFF_VISIBLE | TFF_CLIPPED:

            if ((clipped = ZbFaceClip(fp, tfp, zb.clip_mask, &nclipped)))
            {

                zb.pr.current = BrIntToFixed(BR_RED(tfp->surface));
                zb.pg.current = BrIntToFixed(BR_GRN(tfp->surface));
                zb.pb.current = BrIntToFixed(BR_BLU(tfp->surface));

                ZbFaceRender(clipped, nclipped);
            }
            break;
        }
    }
}

/*
 * Do per-vertex paramter calculations (intensity, u & v)
 */
void ZbFindVertexParameters(void)
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

        if (zb.material->prep_flags & MATUF_SURFACE_VERTICES &&
            !((zb.model->prep_flags & MODUF_VERTEX_GROUPS_MATCH) && zb.face_group_counts[g] == 0))
        {

            SurfacePerMaterial(zb.material);

            if (zb.material->flags & BR_MATF_TWO_SIDED)
            {
                /*
                 * Two sided material - do lighting calculations
                 * with forward facing normals unless the vertex
                 * is only used reversed, then use the negated
                 * normal
                 */
                for (gv = 0, vp = gp->vertices; gv < gp->nvertices; gv++, v++, avp++, vp++)
                {
                    if (zb.vertex_counts[v])
                    {
                        if (avp->direction & TVDIR_FRONT)
                            fw.surface_fn(vp, &vp->n, avp->comp);
                        else
                        {
                            br_fvector3 rev_normal;
                            BrFVector3Negate(&rev_normal, &vp->n);
                            fw.surface_fn(vp, &rev_normal, avp->comp);
                        }
                    }
                }
            }
            else
            {
                for (gv = 0, vp = gp->vertices; gv < gp->nvertices; gv++, v++, avp++, vp++)
                    if (zb.vertex_counts[v])
                        fw.surface_fn(vp, &vp->n, avp->comp);
            }
        }
        else
        {
            avp += gp->nvertices;
            v += gp->nvertices;
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
 * On Screen version of geometry
 */

/*
 * Go through model's face list - find forward facing faces and light them
 */

static void ZbOnScreenFindFacesVerts(void)
{
    int f, g, j, v, vt, df;
    br_face *fp = zb.model->prepared_faces;
    br_face_group *gp = zb.model->face_groups;
    struct temp_face *tfp = zb.temp_faces;

    /*
     * Go through each group
     */
    for (g = 0; g < zb.model->nface_groups; g++, gp++)
    {

        zb.material = (gp->material ? gp->material : zb.default_material);

        if (zb.material->prep_flags & MATUF_SURFACE_FACES)
            SurfacePerMaterial(zb.material);

        zb.face_group_counts[g] = 0;

        if (zb.material->flags & BR_MATF_TWO_SIDED)
        {

            if (!zb.directions_cleared)
                ClearDirections();

            for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
            {

                tfp->flag = TFF_VISIBLE;
                df = TVDIR_FRONT;

                if (BrFVector3Dot(&fp->n, &fw.eye_m) < fp->d)
                {
                    tfp->flag |= TFF_REVERSED;
                    df = TVDIR_BACK;
                }

                zb.vertex_counts[fp->vertices[0]] = 1;
                zb.vertex_counts[fp->vertices[1]] = 1;
                zb.vertex_counts[fp->vertices[2]] = 1;

                zb.temp_vertices[fp->vertices[0]].direction |= df;
                zb.temp_vertices[fp->vertices[1]].direction |= df;
                zb.temp_vertices[fp->vertices[2]].direction |= df;

                if (zb.material->prep_flags & MATUF_SURFACE_FACES)
                    tfp->surface =
                        fw.face_surface_fn(zb.model->prepared_vertices + fp->vertices[0], fp, tfp->flag & TFF_REVERSED);
            }

            zb.face_group_counts[g] = gp->nfaces;
        }
        else if (zb.material->flags & BR_MATF_ALWAYS_VISIBLE)
        {

            if (zb.material->prep_flags & MATUF_SURFACE_FACES)
            {
                for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
                {

                    tfp->flag = TFF_VISIBLE;
                    tfp->surface = fw.face_surface_fn(zb.model->prepared_vertices + fp->vertices[0], fp, 0);
                    zb.vertex_counts[fp->vertices[0]] = 1;
                    zb.vertex_counts[fp->vertices[1]] = 1;
                    zb.vertex_counts[fp->vertices[2]] = 1;
                }
            }
            else
            {
                for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
                {
                    tfp->flag = TFF_VISIBLE;
                    zb.vertex_counts[fp->vertices[0]] = 1;
                    zb.vertex_counts[fp->vertices[1]] = 1;
                    zb.vertex_counts[fp->vertices[2]] = 1;
                }
            }

            zb.face_group_counts[g] = gp->nfaces;
        }
        else
        {

            /*
             * Single sided faces
             */
            if (zb.material->prep_flags & MATUF_SURFACE_FACES)
            {
#if FAST_CULL
                zb.face_group_counts[g] = ZbOSFFVGroupCulledLit_A(fp, tfp, gp->nfaces);
                tfp += gp->nfaces;
                fp += gp->nfaces;
#else
                for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
                {

                    if (BrFVector3Dot(&fp->n, &fw.eye_m) < fp->d)
                    {
                        tfp->flag = 0;
                    }
                    else
                    {
                        tfp->flag = TFF_VISIBLE;
                        zb.face_group_counts[g]++;
                        zb.vertex_counts[fp->vertices[0]] = 1;
                        zb.vertex_counts[fp->vertices[1]] = 1;
                        zb.vertex_counts[fp->vertices[2]] = 1;

                        tfp->surface = fw.face_surface_fn(zb.model->prepared_vertices + fp->vertices[0], fp, 0);
                    }
                }
#endif
            }
            else
            {
#if FAST_CULL
                zb.face_group_counts[g] = ZbOSFFVGroupCulled_A(fp, tfp, gp->nfaces);
                tfp += gp->nfaces;
                fp += gp->nfaces;
#else
                for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
                {
                    if (BrFVector3Dot(&fp->n, &fw.eye_m) < fp->d)
                    {
                        tfp->flag = 0;
                    }
                    else
                    {
                        tfp->flag = TFF_VISIBLE;
                        zb.face_group_counts[g]++;
                        zb.vertex_counts[fp->vertices[0]] = 1;
                        zb.vertex_counts[fp->vertices[1]] = 1;
                        zb.vertex_counts[fp->vertices[2]] = 1;
                    }
                }
#endif
            }
        }
    }
}

static void ZbOnScreenFindFacesVertsPar(void)
{
    int f, g, j, v, vt, df;
    br_face *fp = zb.model->prepared_faces;
    br_face_group *gp = zb.model->face_groups;
    struct temp_face *tfp = zb.temp_faces;

    /*
     * Go through each group
     */
    for (g = 0; g < zb.model->nface_groups; g++, gp++)
    {

        zb.material = (gp->material ? gp->material : zb.default_material);

        if (zb.material->prep_flags & MATUF_SURFACE_FACES)
            SurfacePerMaterial(zb.material);

        zb.face_group_counts[g] = 0;

        if (zb.material->flags & BR_MATF_TWO_SIDED)
        {

            if (!zb.directions_cleared)
                ClearDirections();

            for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
            {

                tfp->flag = TFF_VISIBLE;
                df = TVDIR_FRONT;

                if (BrFVector3Dot(&fp->n, &fw.eye_m) < S0)
                {
                    tfp->flag |= TFF_REVERSED;
                    df = TVDIR_BACK;
                }

                zb.face_group_counts[g]++;

                zb.vertex_counts[fp->vertices[0]] = 1;
                zb.vertex_counts[fp->vertices[1]] = 1;
                zb.vertex_counts[fp->vertices[2]] = 1;

                zb.temp_vertices[fp->vertices[0]].direction |= df;
                zb.temp_vertices[fp->vertices[1]].direction |= df;
                zb.temp_vertices[fp->vertices[2]].direction |= df;

                if (zb.material->prep_flags & MATUF_SURFACE_FACES)
                    tfp->surface =
                        fw.face_surface_fn(zb.model->prepared_vertices + fp->vertices[0], fp, tfp->flag & TFF_REVERSED);
            }
        }
        else if (zb.material->flags & BR_MATF_ALWAYS_VISIBLE)
        {
            /*
             * Two sided faces are always visible
             */
            if (zb.material->prep_flags & MATUF_SURFACE_FACES)
            {
                for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
                {

                    tfp->flag = TFF_VISIBLE;
                    tfp->surface = fw.face_surface_fn(zb.model->prepared_vertices + fp->vertices[0], fp, 0);
                    zb.vertex_counts[fp->vertices[0]] = 1;
                    zb.vertex_counts[fp->vertices[1]] = 1;
                    zb.vertex_counts[fp->vertices[2]] = 1;
                }
            }
            else
            {
                for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
                {
                    tfp->flag = TFF_VISIBLE;
                    zb.vertex_counts[fp->vertices[0]] = 1;
                    zb.vertex_counts[fp->vertices[1]] = 1;
                    zb.vertex_counts[fp->vertices[2]] = 1;
                }
            }

            zb.face_group_counts[g] = gp->nfaces;
        }
        else
        {

            /*
             * Single sided faces
             */
            if (zb.material->prep_flags & MATUF_SURFACE_FACES)
            {
                for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
                {

                    if (BrFVector3Dot(&fp->n, &fw.eye_m) < S0)
                    {
                        tfp->flag = 0;
                    }
                    else
                    {
                        tfp->flag = TFF_VISIBLE;
                        zb.face_group_counts[g]++;
                        zb.vertex_counts[fp->vertices[0]] = 1;
                        zb.vertex_counts[fp->vertices[1]] = 1;
                        zb.vertex_counts[fp->vertices[2]] = 1;

                        tfp->surface = fw.face_surface_fn(zb.model->prepared_vertices + fp->vertices[0], fp, 0);
                    }
                }
            }
            else
            {
                for (f = 0; f < gp->nfaces; f++, fp++, tfp++)
                {
                    if (BrFVector3Dot(&fp->n, &fw.eye_m) <= S0)
                    {
                        tfp->flag = 0;
                    }
                    else
                    {
                        tfp->flag = TFF_VISIBLE;
                        zb.face_group_counts[g]++;
                        zb.vertex_counts[fp->vertices[0]] = 1;
                        zb.vertex_counts[fp->vertices[1]] = 1;
                        zb.vertex_counts[fp->vertices[2]] = 1;
                    }
                }
            }
        }
    }
}

/*
 * For all the active vertices, transform them and work out
 * the per vertex parameters
 */
static void ZbOnScreenTransformVertices(void)
{
    int v, g, gv;
    struct temp_vertex *avp;
    br_vertex *vp;
    br_vertex_group *gp = zb.model->vertex_groups;
    br_vector4 screen;

    /*
     * Base vertices
     */
    v = 0;
    avp = zb.temp_vertices;

    for (g = 0; g < zb.model->nvertex_groups; g++, gp++)
    {

        zb.material = (gp->material ? gp->material : zb.default_material);

        /*
         * Skip if group is not visible
         */
        if ((zb.model->prep_flags & MODUF_VERTEX_GROUPS_MATCH) && zb.face_group_counts[g] == 0)
        {
            avp += gp->nvertices;
            v += gp->nvertices;
            continue;
        }

        /*
         * Select a transform according to current situation
         */

        if (zb.material->flags & BR_MATF_TWO_SIDED)
        {
            if (zb.material->prep_flags & MATUF_SURFACE_VERTICES)
                SurfacePerMaterial(zb.material);

            /*
             * Do surface function and bounds update per vertex
             */
            for (gv = 0, vp = gp->vertices; gv < gp->nvertices; gv++, v++, avp++, vp++)
            {
                if (zb.vertex_counts[v] == 0)
                    continue;

                TRANSFORM_VERTEX_OS();
                avp->comp[C_W] = screen.v[W];
                PROJECT_VERTEX(avp, screen.v[X], screen.v[Y], screen.v[Z], screen.v[W]);
                UPDATE_BOUNDS(*avp);

                if (zb.material->prep_flags & MATUF_SURFACE_VERTICES)
                {
                    if (avp->direction & TVDIR_FRONT)
                        fw.surface_fn(vp, &vp->n, avp->comp);
                    else
                    {
                        br_fvector3 rev_normal;
                        BrFVector3Negate(&rev_normal, &vp->n);
                        fw.surface_fn(vp, &rev_normal, avp->comp);
                    }
                }
            }

            continue;
        }

        if (zb.material->prep_flags & MATUF_SURFACE_VERTICES)
        {

            SurfacePerMaterial(zb.material);

            if (zb.bounds_call)
            {

#if FAST_PROJECT
                ZbOSTVGroupLitBC_A(gp->vertices, (struct temp_vertex_fixed *)avp, gp->nvertices,
                                   (br_uint_8 *)zb.vertex_counts + v);
                avp += gp->nvertices;
                v += gp->nvertices;
#else
                /*
                 * Do surface function and bounds update per vertex
                 */
                for (gv = 0, vp = gp->vertices; gv < gp->nvertices; gv++, v++, avp++, vp++)
                {
                    if (zb.vertex_counts[v] == 0)
                        continue;

                    TRANSFORM_VERTEX_OS();
                    avp->comp[C_W] = screen.v[W];
                    PROJECT_VERTEX(avp, screen.v[X], screen.v[Y], screen.v[Z], screen.v[W]);
                    UPDATE_BOUNDS(*avp);

                    fw.surface_fn(vp, &vp->n, avp->comp);
                }
#endif
            }
            else
            {

                /*
                 * Do surface function  per vertex
                 */
#if FAST_PROJECT
                ZbOSTVGroupLit_A(gp->vertices, (struct temp_vertex_fixed *)avp, gp->nvertices,
                                 (br_uint_8 *)zb.vertex_counts + v);
                avp += gp->nvertices;
                v += gp->nvertices;
#else
                for (gv = 0, vp = gp->vertices; gv < gp->nvertices; gv++, v++, avp++, vp++)
                {
                    if (zb.vertex_counts[v] == 0)
                        continue;

                    TRANSFORM_VERTEX_OS();
                    avp->comp[C_W] = screen.v[W];
                    PROJECT_VERTEX(avp, screen.v[X], screen.v[Y], screen.v[Z], screen.v[W]);

                    fw.surface_fn(vp, &vp->n, avp->comp);
                }
#endif
            }
        }
        else
        {
            if (zb.bounds_call)
            {
                /*
                 * Do bounds update per vertex
                 */
#if FAST_PROJECT
                ZbOSTVGroupBC_A(gp->vertices, (struct temp_vertex_fixed *)avp, gp->nvertices,
                                (br_uint_8 *)zb.vertex_counts + v);
                avp += gp->nvertices;
                v += gp->nvertices;
#else
                for (gv = 0, vp = gp->vertices; gv < gp->nvertices; gv++, v++, avp++, vp++)
                {
                    if (zb.vertex_counts[v] == 0)
                        continue;

                    TRANSFORM_VERTEX_OS();
                    avp->comp[C_W] = screen.v[W];
                    PROJECT_VERTEX(avp, screen.v[X], screen.v[Y], screen.v[Z], screen.v[W]);
                    UPDATE_BOUNDS(*avp);
                }
#endif
            }
            else
            {
                /*
                 * Just transform
                 */
#if FAST_PROJECT
                ZbOSTVGroup_A(gp->vertices, (struct temp_vertex_fixed *)avp, gp->nvertices,
                              (br_uint_8 *)zb.vertex_counts + v);
                avp += gp->nvertices;
                v += gp->nvertices;
#else
                for (gv = 0, vp = gp->vertices; gv < gp->nvertices; gv++, v++, avp++, vp++)
                {
                    if (zb.vertex_counts[v] == 0)
                        continue;

                    TRANSFORM_VERTEX_OS();
                    avp->comp[C_W] = screen.v[W];
                    PROJECT_VERTEX(avp, screen.v[X], screen.v[Y], screen.v[Z], screen.v[W]);
                }
#endif
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
 * Render a model to the screen through the current transform
 */
void ZbMeshRender(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen)
{
    void *scratch;
    int scratch_size;
    char *sp, *clear_end;

    /*
     * Remember model and material in renderer info.
     */
    zb.model = model;
    zb.default_material = material;

#if 0
	{
		extern br_material test_materials[];
		
		if(on_screen == OSC_ACCEPT)
			zb.default_material = test_materials+2;
	}
#endif

#if 0
	on_screen = OSC_PARTIAL;
#endif

    zb.directions_cleared = 0;

    /*
     * Work out amount of scratch space needed for this model
     */
    scratch_size = SCRATCH_ALIGN(model->nprepared_faces * sizeof(*zb.temp_faces));
    scratch_size += SCRATCH_ALIGN(model->nprepared_vertices * sizeof(*zb.vertex_counts));
    scratch_size += SCRATCH_ALIGN(model->nprepared_vertices * sizeof(*zb.temp_vertices));
    scratch_size += SCRATCH_ALIGN(model->nface_groups * sizeof(*zb.face_group_counts));
    scratch_size += SCRATCH_ALIGN(model->nface_groups * sizeof(*zb.face_group_clipped));

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

    zb.face_group_clipped = (void *)sp;

    sp += SCRATCH_ALIGN(model->nface_groups * sizeof(*zb.face_group_clipped));

    clear_end = sp;

    zb.face_group_counts = (void *)sp;
    sp += SCRATCH_ALIGN(model->nface_groups * sizeof(*zb.face_group_counts));

    zb.temp_faces = (void *)sp;
    sp += SCRATCH_ALIGN(model->nprepared_faces * sizeof(*zb.temp_faces));

    zb.temp_vertices = (void *)sp;

    /*
     * Clear vertex counts and vertex materials
     */
    BrBlockFill(scratch, 0, ((clear_end - (char *)scratch) + 3) / 4);

#if FAST_PROJECT
    /*
     * If on_screen, then let the fast project code pre process the model->screen
     * matrix
     *
     * If there was an overflow, revert back to OSC_PARTIAL
     */
    if (on_screen == OSC_ACCEPT)
        if (ZbOSCopyModelToScreen_A())
            on_screen = OSC_PARTIAL;
#endif

    /*
     * Build view_to_model
     *
     * Record a flag to say if model is inside out (det of Xfrm is < 0)
     */
    BrMatrix34Inverse(&fw.view_to_model, &fw.model_to_view);

    /*
     * Transform eye point into model space
     */
    BrVector3EyeInModel(&fw.eye_m);

    /*
     * Normalised version for environment mapping
     */
    BrVector3Normalise(&fw.eye_m_normalised, &fw.eye_m);

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
    if (on_screen == OSC_ACCEPT)
    {

        switch (fw.vtos_type)
        {
        case BR_VTOS_PERSPECTIVE:
            ZbOnScreenFindFacesVerts();
            break;

        case BR_VTOS_PARALLEL:
            ZbOnScreenFindFacesVertsPar();
            break;
        }
        ZbOnScreenTransformVertices();
        ZbRenderFaces();
    }
    else
    {
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
        ZbFindFacesAndVertices();
        ZbFindVertexParameters();
        ZbRenderFaces();
    }

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

/*
 * Attempt to shrink scratch space if current usage is well
 * bellow that allocated
 *
 * If max usage is  l.t. 1/2 allocated space for 50 tests, then
 * resize buffer
 */

void ZbScratchShrink(void)
{
    /* XXX Add Water */
}

#if !BASED_FIXED
/*
 * Convert component from scalar to fixed
 */
void ZbConvertComponents(br_fixed_ls *dest, br_scalar *src, br_uint_32 mask)
{
    for (; mask; mask >>= 1, dest++, src++)
        if (mask & 1)
            *dest = BrScalarToFixed(*src);
}
#endif

/*
 * Local Variables:
 * tab-width: 4
 * End:
 */


