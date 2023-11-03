/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: prepmesh.c 1.35 1995/08/31 16:29:43 sam Exp $
 * $Locker: sam $
 *
 * Precompute information for triangle meshes:
 *	Face normals
 *	Vertex normals
 *	Edge numbers
 *	Bounding radius
 */
#include <string.h>
#include <math.h>

#if DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

#include "fw.h"
#include "brassert.h"
#include "shortcut.h"

static char rscid[] = "$Id: prepmesh.c 1.35 1995/08/31 16:29:43 sam Exp $";

#define SORT_MAT_TYPES 0

/*
 * Temporary structure used whilst processing normals and groups
 */
struct group_temp_vertex
{
    br_material *m;
    br_vertex *v;
    br_int_32 group;
    br_int_32 index;
    br_face *fp;
    br_vector3 n;

    /* XXX Should pack down to
                br_vector3 n;
                br_int_16 vertex;
                br_int_16 face;
     */
};

/*
 * Scratch space for edges - the temporary edge structures are hashed
 * on their first vertex - all similar vertices are the singly linked in
 * a chain
 */
struct pm_temp_edge
{
    struct pm_temp_edge *next; /* next in chain */
    short first;               /* First vertex */
    short last;                /* Last Vertex */
    char other;                /* Edge is used in other direction */
};

static struct pm_temp_edge *pm_edge_table;
static struct pm_temp_edge **pm_edge_hash;

static char *pm_edge_scratch;
static int num_edges = 0;

static int FwAddEdge(short first, short last)
{
    struct pm_temp_edge *tep;

    /*
     * See if edge exists and can be used in other direction
     */
    for (tep = pm_edge_hash[last]; tep; tep = tep->next)
    {

        if (tep->last == first && tep->other == 0)
        {
            /*
             * Yup, flag as used and return index
             */
            tep->other = 1;
            return tep - pm_edge_table;
        }
    }

    /*
     * Create new edge
     */
    tep = pm_edge_table + num_edges;

    tep->first = first;
    tep->last = last;
    tep->other = 0;
    tep->next = pm_edge_hash[first];
    pm_edge_hash[first] = tep;

    return num_edges++;
}

/*
 * Comparison function for qsorting pointers to faces
 */
static int BR_CALLBACK FacesCompare(const void *p1, const void *p2)
{
    const br_face *f1 = *(br_face **)p1, *f2 = *(br_face **)p2;

#if SORT_MAT_TYPES
    int flags1, flags2;

    flags1 = f1->material ? f1->material->flags : 0;
    flags2 = f2->material ? f2->material->flags : 0;

    if (flags1 > flags2)
        return 1;
    if (flags1 < flags2)
        return -1;
#endif

    if (f1->material > f2->material)
        return 1;
    if (f1->material < f2->material)
        return -1;

    return 0;
}

/*
 * Compare temp vertices by  X,Y,Z
 */
static int BR_CALLBACK TVCompare_XYZ(const void *p1, const void *p2)
{
    const struct group_temp_vertex *tv1 = *(struct group_temp_vertex **)p1;
    const struct group_temp_vertex *tv2 = *(struct group_temp_vertex **)p2;
    int i;

    if (tv1->v == tv2->v)
        return 0;

    for (i = 0; i < 3; i++)
    {
        if (tv1->v->p.v[i] > tv2->v->p.v[i])
            return 1;
        if (tv1->v->p.v[i] < tv2->v->p.v[i])
            return -1;
    }

    return 0;
}

#if 0
/*
 * Compare temp vertices by vertex pointer
 */
static int BR_CALLBACK TVCompare_V(const void *p1, const void *p2)
{
	const struct group_temp_vertex *tv1 = *(struct group_temp_vertex **)p1;
	const struct group_temp_vertex *tv2 = *(struct group_temp_vertex **)p2;

	if(tv1->v > tv2->v)
		return 1;
	if(tv1->v < tv2->v)
		return -1;

	return 0;
}
#endif

/*
 * Compare temp vertices by Material, X,Y,Z,U,V Normal
 */
static int BR_CALLBACK TVCompare_MXYZUVN(const void *p1, const void *p2)
{
    const struct group_temp_vertex *tv1 = *(struct group_temp_vertex **)p1;
    const struct group_temp_vertex *tv2 = *(struct group_temp_vertex **)p2;
    int i;

#if SORT_MAT_TYPES
    int flags1, flags2;

    flags1 = tv1->m ? tv1->m->flags : 0;
    flags2 = tv2->m ? tv2->m->flags : 0;

    /*
     * Material
     */
    if (flags1 > flags2)
        return 1;
    if (flags1 < flags2)
        return -1;
#endif

    if (tv1->m > tv2->m)
        return 1;
    if (tv1->m < tv2->m)
        return -1;

    if (tv1->v != tv2->v)
    {
        /*
         * X,Y,Z
         */
        for (i = 0; i < 3; i++)
        {
            if (tv1->v->p.v[i] > tv2->v->p.v[i])
                return 1;
            if (tv1->v->p.v[i] < tv2->v->p.v[i])
                return -1;
        }

        /*
         * U,V
         */
        for (i = 0; i < 2; i++)
        {
            if (tv1->v->map.v[i] > tv2->v->map.v[i])
                return 1;
            if (tv1->v->map.v[i] < tv2->v->map.v[i])
                return -1;
        }
    }

    /*
     * Normal
     */
    for (i = 0; i < 3; i++)
    {
        if (tv1->n.v[i] > tv2->n.v[i])
            return 1;
        if (tv1->n.v[i] < tv2->n.v[i])
            return -1;
    }

    return 0;
}

/*
 * Compare temp vertices by material, vertex pointer, Normal
 */
static int BR_CALLBACK TVCompare_MVN(const void *p1, const void *p2)
{
    const struct group_temp_vertex *tv1 = *(struct group_temp_vertex **)p1;
    const struct group_temp_vertex *tv2 = *(struct group_temp_vertex **)p2;
    int i;

#if SORT_MAT_TYPES
    int flags1, flags2;

    flags1 = tv1->m ? tv1->m->flags : 0;
    flags2 = tv2->m ? tv2->m->flags : 0;

    /*
     * Material
     */
    if (flags1 > flags2)
        return 1;
    if (flags1 < flags2)
        return -1;
#endif

    /*
     * Material
     */
    if (tv1->m > tv2->m)
        return 1;
    if (tv1->m < tv2->m)
        return -1;

    if (tv1->v > tv2->v)
        return 1;
    if (tv1->v < tv2->v)
        return -1;

    /*
     * Normal
     */
    for (i = 0; i < 3; i++)
    {
        if (tv1->n.v[i] > tv2->n.v[i])
            return 1;
        if (tv1->n.v[i] < tv2->n.v[i])
            return -1;
    }

    return 0;
}

/*
 * Work out face normals for original faces
 */
static void PlaneEqn(br_vector3 *v0, br_vector3 *v1, br_vector3 *v2, br_fvector3 *fn, br_scalar *d)
{
#if 0
	br_vector3 a,b,n;

	BrVector3Sub(&a,v1,v0);
	BrVector3Sub(&b,v2,v0);
	BrVector3Cross(&n,&a,&b);
	BrFVector3Normalise(fn,&n);

	*d = BrFVector3Dot(fn,v0);
#endif

#if 1
    /*
     * Do plane equation calcs. in floating point to
     * get the accuracy
     */
    br_vector3 a, b;
    float ax, ay, az;
    float bx, by, bz;
    float nx, ny, nz;
    float l;

    BrVector3Sub(&a, v1, v0);
    BrVector3Sub(&b, v2, v0);

    ax = BrScalarToFloat(a.v[0]);
    ay = BrScalarToFloat(a.v[1]);
    az = BrScalarToFloat(a.v[2]);

    bx = BrScalarToFloat(b.v[0]);
    by = BrScalarToFloat(b.v[1]);
    bz = BrScalarToFloat(b.v[2]);

    nx = ay * bz - az * by;
    ny = az * bx - ax * bz;
    nz = ax * by - ay * bx;

    l = sqrt(nx * nx + ny * ny + nz * nz);

    if (l != 0)
    {
        l = 1.0 / l;
        nx *= l;
        ny *= l;
        nz *= l;

        fn->v[0] = BR_FRACTION(nx);
        fn->v[1] = BR_FRACTION(ny);
        fn->v[2] = BR_FRACTION(nz);
    }
    else
    {
        fn->v[0] = BR_FRACTION(0.0);
        fn->v[1] = BR_FRACTION(0.0);
        fn->v[2] = BR_FRACTION(1.0);
    }

    *d = BrFVector3Dot(fn, v0);
#endif
}

static void PrepareFaceNormals(br_model *model)
{
    br_vertex *vertices = model->vertices;
    br_face *fp;
    int f;

    ASSERT(model->faces && model->vertices);

    fp = model->faces;
    for (f = 0; f < model->nfaces; f++, fp++)
        PlaneEqn(&vertices[fp->vertices[0]].p, &vertices[fp->vertices[1]].p, &vertices[fp->vertices[2]].p, &fp->n,
                 &fp->d);
}

/*
 * Work out face normals for prepared faces
 */
static void PrepareFaceNormalsFast(br_model *model)
{
    br_vector3 a, b, n;
    br_vertex *vertices = model->prepared_vertices;
    br_face *fp;
    int f;

    ASSERT(model->prepared_faces && model->prepared_vertices);

    fp = model->prepared_faces;
    for (f = 0; f < model->nfaces; f++, fp++)
        PlaneEqn(&vertices[fp->vertices[0]].p, &vertices[fp->vertices[1]].p, &vertices[fp->vertices[2]].p, &fp->n,
                 &fp->d);
}

/*
 * Work out vertex normals for prepared vertices - ignores smoothing groups
 */
static void PrepareVertexNormalsFast(br_model *model)
{
    int f, v;
    br_face *fp;
    br_vertex *vp;
    br_vector3 *vertex_sums, *vsp;

    /*
     * Allocate vertex normal sums
     */
    vertex_sums = BrScratchAllocate(model->nprepared_vertices * sizeof(*vertex_sums));
    memset(vertex_sums, 0, model->nprepared_vertices * sizeof(*vertex_sums));

    /*
     * For each face, add face's normal to it's attached vertices
     */
    fp = model->prepared_faces;
    for (f = 1; f < model->nprepared_faces; f++, fp++)
    {
        BrVector3Accumulate(vertex_sums + fp->vertices[0], &fp->n);
        BrVector3Accumulate(vertex_sums + fp->vertices[1], &fp->n);
        BrVector3Accumulate(vertex_sums + fp->vertices[2], &fp->n);
    }

    /*
     * Normalise the accumulated vectors into the vertices
     */
    vp = model->prepared_vertices;
    vsp = vertex_sums;

    for (v = 0; v < model->nprepared_vertices; v++, vp++, vsp++)
    {
        BrFVector3Normalise(&vp->n, vsp);
    }

    BrScratchFree(vertex_sums);
}

/*
 * Rearrange faces into groups by material
 */
static void PrepareFaces(br_model *model)
{
    int f, g, count;
    br_face **face_ptrs;

    ASSERT(model != NULL);
    ASSERT(model->faces != NULL);

    /*
     * Undo any sharing of prepared faces
     */
    if (model->faces == model->prepared_faces)
    {
        ASSERT(model->nfaces == model->nprepared_faces);
        model->prepared_faces = NULL;
        model->nprepared_faces = 0;
    }

    /*
     * Release any previous array if it is not the right size
     */
    if (model->prepared_faces && (model->nfaces != model->nprepared_faces))
    {
        BrResFree(model->prepared_faces);
        model->prepared_faces = NULL;
        model->nprepared_faces = 0;
    }

    /*
     * Allocate a prepared_faces table of the right size
     */
    if (model->prepared_faces == NULL)
    {

        model->prepared_faces =
            BrResAllocate(model, model->nfaces * sizeof(*model->prepared_faces), BR_MEMORY_PREPARED_FACES);

        model->nprepared_faces = model->nfaces;
    }

    ASSERT(model->nprepared_faces == model->nfaces);

    /*
     * Sort the faces based on (if available) material type,and then material pointer
     */
    face_ptrs = BrScratchAllocate(sizeof(*face_ptrs) * model->nprepared_faces);

    for (f = 0; f < model->nfaces; f++)
        face_ptrs[f] = model->faces + f;

    BrQsort(face_ptrs, model->nprepared_faces, sizeof(*face_ptrs), FacesCompare);

    /*
     * Build face tags array
     */
    if (model->flags & BR_MODF_GENERATE_TAGS)
    {

        if (model->face_tags)
            BrResFree(model->face_tags);

        model->face_tags = BrResAllocate(model, sizeof(*model->face_tags) * model->nprepared_faces, BR_MEMORY_SCRATCH);

        for (f = 0; f < model->nprepared_faces; f++)
            model->face_tags[f] = face_ptrs[f] - model->faces;
    }

    /*
     * Build new face table
     */
    for (f = 0; f < model->nprepared_faces; f++)
    {
        model->prepared_faces[f] = *face_ptrs[f];
    }

    /*
     * Release scratch
     */
    BrScratchFree(face_ptrs);

    /*
     * Release any old faces
     */
    if (!(model->flags & BR_MODF_KEEP_ORIGINAL))
    {
        ASSERT(model->faces);
        BrResFree(model->faces);
        model->faces = model->prepared_faces;
        model->nfaces = model->nprepared_faces;
    }
}

/*
 * Don't do any sorting, the prepared faces are the same as the faces
 */
static void PrepareFacesFast(br_model *model)
{
    int f, g, count;

    /*
     * If there is a sorted copy of the faces, ditch it
     */
    if (model->prepared_faces && (model->prepared_faces != model->faces))
        BrResFree(model->prepared_faces);

    model->nprepared_faces = model->nfaces;
    model->prepared_faces = model->faces;

    /*
     * Build face tags array
     */
    if (model->flags & BR_MODF_GENERATE_TAGS)
    {

        if (model->face_tags)
            BrResFree(model->face_tags);

        model->face_tags = BrResAllocate(model, sizeof(*model->face_tags) * model->nprepared_faces, BR_MEMORY_SCRATCH);

        for (f = 0; f < model->nprepared_faces; f++)
            model->face_tags[f] = f;
    }
}

static void PrepareFaceGroups(br_model *model)
{
    int f, g, count;

    /*
     * Go through sorted faces and find number of groups
     */
    for (f = 1, g = 1; f < model->nprepared_faces; f++)
        if (model->prepared_faces[f].material != model->prepared_faces[f - 1].material)
            g++;

    /*
     * Allocate and fill in face group table
     */
    if (model->face_groups != NULL && model->nface_groups != g)
    {
        /*
         * Remove any old table
         */
        BrResFree(model->face_groups);
        model->face_groups = NULL;
    }

    if (model->face_groups == NULL)
        model->face_groups = BrResAllocate(model, g * sizeof(*model->face_groups), BR_MEMORY_GROUPS);

    model->nface_groups = g;
    model->face_groups[0].faces = model->prepared_faces;

    for (f = 1, g = 0, count = 1; f < model->nprepared_faces; f++, count++)
    {
        if (model->prepared_faces[f].material != model->prepared_faces[f - 1].material)
        {
            model->face_groups[g].material = model->prepared_faces[f - 1].material;
            model->face_groups[g].nfaces = count;
            model->face_groups[g + 1].faces = model->prepared_faces + f;
            count = 0;
            g++;
        }
    }
    model->face_groups[g].material = model->prepared_faces[f - 1].material;
    model->face_groups[g].nfaces = count;
}

/*
 * Given a group of temp_vertices that are deemeed to be co-incident,
 * accumulate the normals for each vertex by adding all
 * normals in the groups that share at least 1 smoothing group
 */
static void AccumulateSmoothing(struct group_temp_vertex **start, struct group_temp_vertex **end)
{
    struct group_temp_vertex **outer, **inner;

    for (outer = start; outer < end; outer++)
        for (inner = start; inner < end; inner++)
            if ((*outer)->fp->smoothing & (*inner)->fp->smoothing)
                BrVector3Accumulate(&(*outer)->n, &(*inner)->fp->n);
}

/*
 * Generate the prepared vertex table - split vertices at material and smoothing group boundaries
 */
br_material **PrepareVertices(br_model *model)
{
    struct group_temp_vertex *temp_verts, *gtvp, **sorted;
    int f, v, count, i, ntemps, num_shared, simple_smoothing;
    br_face *fp;
    br_vertex *old_vertices, *vp;
    br_qsort_cbfn *vertex_compare_1;
    br_qsort_cbfn *vertex_compare_2;
    br_uint_16 *shared_table;
    void *scratch;
    br_size_t scratch_size;
    br_material **materials;

    ASSERT(model != NULL);
    ASSERT(model->vertices != NULL);

    /*
     * Undo any sharing of prepared vertices
     */
    if (model->vertices == model->prepared_vertices)
    {
        ASSERT(model->nvertices == model->nprepared_vertices);
        model->prepared_vertices = NULL;
        model->nprepared_vertices = 0;
    }

    if (model->flags & BR_MODF_DONT_WELD)
    {
        vertex_compare_1 = TVCompare_XYZ;
        vertex_compare_2 = TVCompare_MVN;
    }
    else
    {
        vertex_compare_1 = TVCompare_XYZ;
        vertex_compare_2 = TVCompare_MXYZUVN;
    }

    ASSERT(model->vertices != NULL && model->nvertices != 0);

    /*
     * Create a per face vertex table, and a table of pointers
     * to each member, which is then sorted in various ways
     */
    ntemps = model->nprepared_faces * 3;

    scratch_size = ntemps * (sizeof(*temp_verts) + sizeof(*sorted));
    scratch = BrScratchAllocate(scratch_size);

    memset(scratch, 0, scratch_size);

    temp_verts = scratch;
    sorted = (struct group_temp_vertex **)(temp_verts + ntemps);

    gtvp = temp_verts;

    for (i = 0, f = 0, fp = model->prepared_faces; f < model->nprepared_faces; f++, fp++)
    {
        for (v = 0; v < 3; v++, i++, gtvp++)
        {

            ASSERT(gtvp < temp_verts + model->nprepared_faces * 3);

            gtvp->m = fp->material;
            gtvp->v = model->vertices + fp->vertices[v];
            gtvp->fp = fp;

            sorted[i] = gtvp;
        }
    }

    ASSERT(i == ntemps);
    ASSERT(gtvp == temp_verts + ntemps);

    /*
     * Accumulate normals for each group of temp verts
     *  that reference the same original vertex
     */
    BrQsort(sorted, ntemps, sizeof(*sorted), vertex_compare_1);

    for (v = 0, i = 0; v < ntemps - 1; v++)
    {

        if (vertex_compare_1(sorted + v, sorted + v + 1))
        {
            /*
             * Process a group of vertices
             */
            AccumulateSmoothing(sorted + i, sorted + v + 1);
            i = v + 1;
        }
    }

    AccumulateSmoothing(sorted + i, sorted + ntemps);

    /*
     * Resort pointers by material, vertex, normal
     */
    BrQsort(sorted, ntemps, sizeof(*sorted), vertex_compare_2);

    /*
     * Count distinct and vertices
     */
    count = 0;
    for (v = 0, i = 0; v < ntemps - 1; v++)
    {

        sorted[v]->index = count;

        /*
         * New Vertex
         */
        if (vertex_compare_2(sorted + v, sorted + v + 1))
            count++;
    }
    sorted[v]->index = count;
    count++;

    /*
     * Remove any previous prepared table
     */
    if (model->prepared_vertices != NULL && model->nprepared_vertices != count)
    {
        BrResFree(model->prepared_vertices);
        model->prepared_vertices = NULL;
        model->nprepared_vertices = 0;
    }

    /*
     * Allocate new vertex table
     */
    if (model->prepared_vertices == NULL)
        model->prepared_vertices = BrResAllocate(model, count * sizeof(br_vertex), BR_MEMORY_PREPARED_VERTICES);
    model->nprepared_vertices = count;

    /*
     * Generate new vertices and an index of materials for each vertex
     */
    count = 0;
    vp = model->prepared_vertices;
    materials = BrResAllocate(model, sizeof(*materials) * model->nprepared_vertices, BR_MEMORY_SCRATCH);

    for (v = 0, i = 0; v < ntemps - 1; v++)
    {

        ASSERT(sorted[v]->index == count);

        /*
         * New Vertex
         */
        if (sorted[v]->index != sorted[v + 1]->index)
        {
            *vp = *sorted[v]->v;
            BrFVector3Normalise(&vp->n, &sorted[v]->n);
            materials[count] = sorted[v]->m;
            count++;
            vp++;
        }
    }

    /*
     * Handle last vertex
     */
    *vp = *sorted[v]->v;
    BrFVector3Normalise(&vp->n, &sorted[v]->n);
    materials[count] = sorted[v]->m;

    /*
     * Relink faces to new vertices
     */
    gtvp = temp_verts;

    for (i = 0, f = 0, fp = model->prepared_faces; f < model->nprepared_faces; f++, fp++)
        for (v = 0; v < 3; v++, i++, gtvp++)
            fp->vertices[v] = gtvp->index;

    /*
     * Build vertex tags array
     */
    if (model->flags & BR_MODF_GENERATE_TAGS)
    {

        if (model->vertex_tags)
            BrResFree(model->vertex_tags);

        model->vertex_tags =
            BrResAllocate(model, sizeof(*model->vertex_tags) * model->nprepared_vertices, BR_MEMORY_SCRATCH);

        for (v = 0; v < ntemps; v++)
            model->vertex_tags[temp_verts[v].index] = temp_verts[v].v - model->vertices;
    }

    /*
     * Free up scratch space
     */
    BrScratchFree(scratch);

    /*
     * Release the old vertices, if not needed
     */
    if (!(model->flags & BR_MODF_KEEP_ORIGINAL))
    {
        if (model->vertices)
            BrResFree(model->vertices);

        /*
         * Share vertices with prepared
         */
        model->vertices = model->prepared_vertices;
        model->nvertices = model->nprepared_vertices;
    }

    return materials;
}

/*
 * Generate the prepared vertex table - quick and dirty
 * May create more vertices than necessary
 */
br_material **PrepareVerticesFast(br_model *model)
{
    struct
    {
        br_vertex *v;
        br_material *m;
    } * temp_verts, **vert_index;
    br_material **materials;
    br_face *fp;
    br_size_t scratch_size;
    int f, v;
    int count = 0;

    ASSERT(model != NULL);
    ASSERT(model->vertices != NULL);

    /*
     * Undo any sharing of prepared vertices
     */
    if (model->vertices == model->prepared_vertices)
    {
        ASSERT(model->nvertices == model->nprepared_vertices);
        model->prepared_vertices = NULL;
        model->nprepared_vertices = 0;
    }

    scratch_size = model->nvertices * sizeof(*vert_index) + model->nprepared_faces * 3 * sizeof(*temp_verts);

    vert_index = BrScratchAllocate(scratch_size);
    temp_verts = (void *)((char *)vert_index + model->nvertices * sizeof(*vert_index));

    memset(vert_index, 0, model->nvertices * sizeof(*vert_index));

    for (f = 0, fp = model->prepared_faces; f < model->nprepared_faces; f++, fp++)
    {
        for (v = 0; v < 3; v++)
        {
            /*
             * See if the vertex has already been allocated, and if the material matches
             */
            if ((vert_index[fp->vertices[v]]) && vert_index[fp->vertices[v]]->m == fp->material)
            {

                fp->vertices[v] = vert_index[fp->vertices[v]] - temp_verts;
                continue;
            }

            /*
             * Build a new vertex
             */
            temp_verts[count].v = model->vertices + fp->vertices[v];
            temp_verts[count].m = fp->material;
            vert_index[fp->vertices[v]] = temp_verts + count;
            fp->vertices[v] = count;

            count++;
        }
    }

    /*
     * Build the new vertex and material tables
     */
    /*
     * Remove any previous prepared table
     */
    if (model->prepared_vertices != NULL && model->nprepared_vertices != count)
    {
        BrResFree(model->prepared_vertices);
        model->prepared_vertices = NULL;
        model->nprepared_vertices = 0;
    }

    /*
     * Allocate new vertex table
     */
    if (model->prepared_vertices == NULL)
        model->prepared_vertices = BrResAllocate(model, count * sizeof(br_vertex), BR_MEMORY_PREPARED_VERTICES);
    model->nprepared_vertices = count;

    materials = BrResAllocate(model, sizeof(*materials) * count, BR_MEMORY_SCRATCH);

    for (v = 0; v < model->nprepared_vertices; v++)
    {
        model->prepared_vertices[v] = *temp_verts[v].v;
        materials[v] = temp_verts[v].m;
    }

    /*
     * Build vertex tags array
     */
    if (model->flags & BR_MODF_GENERATE_TAGS)
    {

        if (model->vertex_tags)
            BrResFree(model->vertex_tags);

        model->vertex_tags =
            BrResAllocate(model, sizeof(*model->vertex_tags) * model->nprepared_vertices, BR_MEMORY_SCRATCH);

        for (v = 0; v < model->nprepared_vertices; v++)
            model->vertex_tags[v] = temp_verts[v].v - model->vertices;
    }

    /*
     * Release scratch space
     */
    BrScratchFree(vert_index);

    /*
     * Release the old vertices, if not needed
     */
    if (!(model->flags & BR_MODF_KEEP_ORIGINAL))
    {
        if (model->vertices)
            BrResFree(model->vertices);

        /*
         * Share vertices with prepared
         */
        model->vertices = model->prepared_vertices;
        model->nvertices = model->nprepared_vertices;
    }

    return materials;
}

static void PrepareVertexGroups(br_model *model, br_material **vertex_materials)
{
    int g, v, count;
    br_vertex *vp;
    br_vertex_group *gp;

    /*
     * Count material groups
     */
    for (v = 1, g = 1; v < model->nprepared_vertices; v++)
        if (vertex_materials[v] != vertex_materials[v - 1])
            g++;

    /*
     * Allocate and fill in vertex group table
     */
    if (model->vertex_groups)
        BrResFree(model->vertex_groups);

    model->vertex_groups = BrResAllocate(model, g * sizeof(*model->vertex_groups), BR_MEMORY_GROUPS);
    model->nvertex_groups = g;

    /*
     * Generate new vertex groups
     */
    gp = model->vertex_groups;
    vp = model->prepared_vertices;

    gp->vertices = vp;
    gp->material = vertex_materials[0];

    for (v = 1, vp++; v < model->nprepared_vertices; v++, vp++)
    {
        if (vertex_materials[v] != vertex_materials[v - 1])
        {
            gp->nvertices = vp - gp->vertices;
            gp++;
            gp->vertices = vp;
            gp->material = vertex_materials[v];
        }
    }

    gp->nvertices = vp - gp->vertices;
}

/*
 * Build edge references for each face
 */
void PrepareEdges(br_model *model)
{
    br_size_t scratch_size;
    br_face *fp;
    int f;

    scratch_size = model->nfaces * 3 * sizeof(*pm_edge_table) + model->nvertices * sizeof(*pm_edge_hash);

    pm_edge_scratch = BrScratchAllocate(scratch_size);

    /*
     * Divvy up scratch area and clear hash pointers
     */
    pm_edge_hash = (struct pm_temp_edge **)pm_edge_scratch;
    pm_edge_table = (struct pm_temp_edge *)(pm_edge_scratch + model->nvertices * sizeof(*pm_edge_hash));

    memset(pm_edge_hash, 0, model->nvertices * sizeof(*pm_edge_hash));

    /*
     * Accumulate shared edges from each face
     */
    num_edges = 0;
    fp = model->faces;

    for (f = 0; f < model->nfaces; f++, fp++)
    {
        fp->edges[0] = FwAddEdge(fp->vertices[0], fp->vertices[1]);
        fp->edges[1] = FwAddEdge(fp->vertices[1], fp->vertices[2]);
        fp->edges[2] = FwAddEdge(fp->vertices[2], fp->vertices[0]);
    }

    model->nedges = num_edges;

    BrScratchFree(pm_edge_scratch);
}

/*
 * Find bounding radius of model
 */
static void PrepareBoundingRadius(br_model *model)
{
    br_scalar d, max = BR_SCALAR(0.0);
    int v;
    br_vertex *vp;

    /*
     * XXX this is really dodgy in fixed point
     */
    for (v = 0, vp = model->vertices; v < model->nvertices; v++, vp++)
    {
        d = BrVector3LengthSquared(&vp->p);

        if (d > max)
            max = d;
    }

    model->radius = BR_SQRT(max);
}

#if DEBUG
void VertexDump(char *prefix, br_vertex *vp)
{
    BrLogPrintf("%sp=(% 5.2g,% 5.2g,% 5.2g) map=(% 5.2g,% 5.2g) n=(% 5.2g,% 5.2g,% 5.2g) r=%d\n", prefix,
                BrScalarToFloat(vp->p.v[X]), BrScalarToFloat(vp->p.v[Y]), BrScalarToFloat(vp->p.v[Z]),
                BrScalarToFloat(vp->map.v[X]), BrScalarToFloat(vp->map.v[Y]),
                BrScalarToFloat(BrFractionToScalar(vp->n.v[X])), BrScalarToFloat(BrFractionToScalar(vp->n.v[Y])),
                BrScalarToFloat(BrFractionToScalar(vp->n.v[Z])), vp->r);
}

void FaceDump(char *prefix, br_face *fp)
{
    BrLogPrintf("%svertices=(%2d,%2d,%2d) edges=(%2d,%2d,%2d) smoothing=%04x flags=%02x material=%s\n", prefix,
                fp->vertices[0], fp->vertices[1], fp->vertices[2], fp->edges[0], fp->edges[1], fp->edges[2],
                fp->smoothing, fp->flags,
                fp->material ? (fp->material->identifier ? fp->material->identifier : "???") : "DEFAULT");
}

void FaceGroupDump(char *prefix, br_face_group *gp, br_face *base_face)
{
    BrLogPrintf("%sfaces = %p (%d) nfaces=%d material=%s\n", prefix, gp->faces, gp->faces - base_face, gp->nfaces,
                gp->material ? (gp->material->identifier ? gp->material->identifier : "???") : "DEFAULT");
}

void VertexGroupDump(char *prefix, br_vertex_group *gp, br_vertex *base_vertex)
{
    BrLogPrintf("%svertices = %p (%d) nvertices=%d material=%s\n", prefix, gp->vertices, gp->vertices - base_vertex,
                gp->nvertices,
                gp->material ? (gp->material->identifier ? gp->material->identifier : "???") : "DEFAULT");
}

void ModelDump(char *prefix, br_model *model)
{
    int v;
    int f;
    char tmp[80];

    /*
     * Print all the model entries
     */
    BrLogPrintf("%sidentifier         = \"%s\"\n", prefix, model->identifier ? model->identifier : "NULL");
    BrLogPrintf("%svertices           = %p\n", prefix, model->vertices);
    BrLogPrintf("%sfaces              = %p\n", prefix, model->faces);
    BrLogPrintf("%snvertices          = %d\n", prefix, model->nvertices);
    BrLogPrintf("%snfaces             = %d\n", prefix, model->nfaces);
    BrLogPrintf("%spivot              = (%g,%g,%g)\n", prefix, BrScalarToFloat(model->pivot.v[X]),
                BrScalarToFloat(model->pivot.v[X]), BrScalarToFloat(model->pivot.v[X]));
    BrLogPrintf("%sflags              = %04x\n", prefix, model->flags);
    BrLogPrintf("%sradius             = %g\n", prefix, BrScalarToFloat(model->radius));
    BrLogPrintf("%snface_groups       = %d\n", prefix, model->nface_groups);
    BrLogPrintf("%snvertex_groups     = %d\n", prefix, model->nvertex_groups);
    BrLogPrintf("%snedges             = %d\n", prefix, model->nedges);
    BrLogPrintf("%snprepared_vertices = %d\n", prefix, model->nprepared_vertices);
    BrLogPrintf("%snprepared_faces    = %d\n", prefix, model->nprepared_faces);
    BrLogPrintf("%sface_groups        = %p\n", prefix, model->face_groups);
    BrLogPrintf("%svertex_groups      = %p\n", prefix, model->vertex_groups);
    BrLogPrintf("%sprepared_vertices  = %p\n", prefix, model->prepared_vertices);
    BrLogPrintf("%sprepared_faces     = %p\n", prefix, model->prepared_faces);

    BrLogPrintf("\n");
    if (model->vertices)
    {
        for (v = 0; v < model->nvertices; v++)
        {
            sprintf(tmp, "model->vertices[%2d].", v);
            VertexDump(tmp, model->vertices + v);
        }
    }

    BrLogPrintf("\n");

    if (model->faces)
    {
        for (f = 0; f < model->nfaces; f++)
        {
            sprintf(tmp, "model->faces[%2d].", f);
            FaceDump(tmp, model->faces + f);
        }
    }

    BrLogPrintf("\n");
    if (model->prepared_vertices)
    {
        for (v = 0; v < model->nprepared_vertices; v++)
        {
            sprintf(tmp, "model->prepared_vertices[%2d].", v);
            VertexDump(tmp, model->prepared_vertices + v);
        }
    }

    BrLogPrintf("\n");

    if (model->prepared_faces)
    {
        for (f = 0; f < model->nprepared_faces; f++)
        {
            sprintf(tmp, "model->prepared_faces[%2d].", f);
            FaceDump(tmp, model->prepared_faces + f);
        }
    }

    BrLogPrintf("\n");

    if (model->face_groups)
    {
        for (f = 0; f < model->nface_groups; f++)
        {
            sprintf(tmp, "model->face_groups[%2d].", f);
            FaceGroupDump(tmp, model->face_groups + f, model->prepared_faces);
        }
    }

    BrLogPrintf("\n");

    if (model->vertex_groups)
    {
        for (v = 0; v < model->nvertex_groups; v++)
        {
            sprintf(tmp, "model->vertex_groups[%2d].", v);
            VertexGroupDump(tmp, model->vertex_groups + v, model->prepared_vertices);
        }
    }
}

#endif

/*
 * Find bounding box of model
 */
static void PrepareBoundingBox(br_model *model)
{
    int axis, v;
    br_vertex *vp;
    br_scalar x;

    /*
     * Initialise bounding box to first vertex
     */
    for (axis = 0; axis < 3; axis++)
        model->bounds.min.v[axis] = model->bounds.max.v[axis] = model->prepared_vertices[0].p.v[axis];

    /*
     * Expand bounding box with remaining vertices
     */
    for (v = 1, vp = model->prepared_vertices + 1; v < model->nprepared_vertices; v++, vp++)
    {
        for (axis = 0; axis < 3; axis++)
        {

            x = vp->p.v[axis];

            if (x > model->bounds.max.v[axis])
                model->bounds.max.v[axis] = x;

            if (x < model->bounds.min.v[axis])
                model->bounds.min.v[axis] = x;
        }
    }
}

/*
 * Do all model preprocessing
 */
void BR_PUBLIC_ENTRY BrModelUpdate(br_model *model, br_uint_16 flags)
{
    int g, f, v, count, axis;
    br_vector3 a, b, n;
    br_vertex *vertices = model->vertices, *vp, *old_vertices;
    br_face *fp;
    br_scalar d, max;
    br_material *mp, **mpp;

#if 0
	BrLogPrintf("Before ModelUpdate...\n");
	ModelDump("    ", model);
#endif

    /*
     * Do not do anything if model is marked as being pre-prepared
     */
    if (model->flags & BR_MODF_PREPREPARED)
        return;

    /*
     * If model has no faces, remove groups and do nothing involving
     * faces
     */
    if (model->faces == NULL || model->vertices == NULL)
        return;

    if (model->nfaces == 0)
    {
        if (model->face_groups)
            BrResFree(model->face_groups);
        model->face_groups = NULL;

        flags &= ~(BR_MODU_NORMALS | BR_MODU_EDGES | BR_MODU_GROUPS);
    }

    /*
     * Check that faces do not reference non-existant vertices
     */
    fp = model->faces;

    for (f = 0; f < model->nfaces; f++, fp++)
        for (v = 0; v < 3; v++)
            if (fp->vertices[v] >= model->nvertices)
                BR_ERROR2("face references invalid vertex f=%d v=%d", f, v);
    /*
     * See if pivot point is not zero
     */
    if ((model->pivot.v[0] != S0) || (model->pivot.v[1] != S0) || (model->pivot.v[2] != S0))
        model->prep_flags |= MODUF_HAS_PIVOT;
    else
        model->prep_flags &= ~MODUF_HAS_PIVOT;

    /*
     * Edge numbers
     */
    if (flags & BR_MODU_EDGES)
        PrepareEdges(model);

    /*
     *	Vertex normals and groups
     */
    if (flags & BR_MODU_GROUPS)
    {

        if (model->flags & BR_MODF_QUICK_UPDATE)
        {
            /*
             * Dont sort faces or vertices
             */
            PrepareFaceNormals(model);

            PrepareFacesFast(model);
            PrepareFaceGroups(model);

            mpp = PrepareVerticesFast(model);
            PrepareVertexGroups(model, mpp);
            BrResFree(mpp);

            PrepareVertexNormalsFast(model);

            model->prep_flags &= ~MODUF_VERTEX_GROUPS_MATCH;
        }
        else
        {
            /*
             * Sort faces into groups the slow way
             */
            PrepareFaceNormals(model);

            PrepareFaces(model);
            PrepareFaceGroups(model);

            mpp = PrepareVertices(model);
            PrepareVertexGroups(model, mpp);
            BrResFree(mpp);

            model->prep_flags |= MODUF_VERTEX_GROUPS_MATCH;
        }

        /*
         * Offset prepared vertices by pivot point
         */
        if (model->prep_flags & MODUF_HAS_PIVOT)
        {
            for (v = 0; v < model->nprepared_vertices; v++)
            {
                model->prepared_vertices[v].p.v[0] -= model->pivot.v[0];
                model->prepared_vertices[v].p.v[1] -= model->pivot.v[1];
                model->prepared_vertices[v].p.v[2] -= model->pivot.v[2];
            }
        }
    }

    /*
     * Bounding radius
     *
     */
    if (flags & BR_MODU_RADIUS)
        PrepareBoundingRadius(model);

    /*
     * Bounding box
     */
    if (flags & BR_MODU_BOUNDING_BOX)
        PrepareBoundingBox(model);

    if ((flags & (BR_MODU_NORMALS | BR_MODU_GROUPS)) == BR_MODU_NORMALS)
    {
        /*
         * Just regenerate normals
         */
        /*
         * Copy XYZ from original faces - if available
         */
        if ((model->flags & (BR_MODF_KEEP_ORIGINAL | BR_MODF_GENERATE_TAGS)) ==
            (BR_MODF_KEEP_ORIGINAL | BR_MODF_GENERATE_TAGS))
        {

            for (v = 0, vp = model->prepared_vertices; v < model->nprepared_vertices; v++, vp++)
            {
                vp->p.v[0] = model->vertices[model->vertex_tags[v]].p.v[0] - model->pivot.v[0];
                vp->p.v[1] = model->vertices[model->vertex_tags[v]].p.v[1] - model->pivot.v[1];
                vp->p.v[2] = model->vertices[model->vertex_tags[v]].p.v[2] - model->pivot.v[2];
            }
        }

        PrepareFaceNormalsFast(model);
        PrepareVertexNormalsFast(model);
    }

    /*
     * Call any renderer function
     */
    if (fw.model_update)
        fw.model_update(model, flags);

#if 0
	BrLogPrintf("After ModelUpdate...\n");
	ModelDump("    ", model);
#endif
}
