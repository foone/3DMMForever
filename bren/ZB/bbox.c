/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: bbox.c 1.22 1995/08/31 16:47:31 sam Exp $
 * $Locker:  $
 *
 * Bounding box rendering
 */

#include "zb.h"
#include "shortcut.h"

static char rscid[] = "$Id: bbox.c 1.22 1995/08/31 16:47:31 sam Exp $";

/*
 * A pre-preocessed static cuboid model that we can fill in with
 * appropriate vertex values to represent a bounding box
 *
 * Another way to do this is to use a cube model, and prepend a scale+xform
 * that matches it to the bounds - this has the disadvantage of making
 * the lighting do funny things unless BR_LIGHT_VIEW is used. I would prefer
 * that the user only has to do this when they themselves have created
 * a funny transform.
 */

/*
 * 8 Vertices
 */
static struct br_vertex zb_bounds_vertices[] = {
    {BR_VECTOR3(-1, -1, -1), BR_VECTOR2(0, 0), 0, 0, 0, 0, 0, BR_FVECTOR3(-0.666, -0.333, -0.666)}, /*    0 */
    {BR_VECTOR3(-1, -1, 1), BR_VECTOR2(0, 0), 0, 0, 0, 0, 0, BR_FVECTOR3(-0.333, -0.666, 0.666)},   /*    1 */
    {BR_VECTOR3(-1, 1, -1), BR_VECTOR2(0, 0), 0, 0, 0, 0, 0, BR_FVECTOR3(-0.408, 0.816, -0.408)},   /*    2 */
    {BR_VECTOR3(-1, 1, 1), BR_VECTOR2(0, 0), 0, 0, 0, 0, 0, BR_FVECTOR3(-0.816, 0.408, 0.408)},     /*    3 */
    {BR_VECTOR3(1, -1, -1), BR_VECTOR2(0, 0), 0, 0, 0, 0, 0, BR_FVECTOR3(0.408, -0.816, -0.408)},   /*    4 */
    {BR_VECTOR3(1, -1, 1), BR_VECTOR2(0, 0), 0, 0, 0, 0, 0, BR_FVECTOR3(0.816, -0.408, 0.408)},     /*    5 */
    {BR_VECTOR3(1, 1, -1), BR_VECTOR2(0, 0), 0, 0, 0, 0, 0, BR_FVECTOR3(0.666, 0.333, -0.666)},     /*    6 */
    {BR_VECTOR3(1, 1, 1), BR_VECTOR2(0, 0), 0, 0, 0, 0, 0, BR_FVECTOR3(0.333, 0.666, 0.666)},       /*    7 */
};

/*
 * 12 Faces
 */
static struct br_face zb_bounds_faces[] = {
    {{5, 6, 7}, {5, 13, 14}, NULL, 1, 1, 0, BR_FVECTOR3(1, 0, 0), BR_SCALAR(1)},  /*    6 */
    {{5, 4, 6}, {3, 4, 5}, NULL, 1, 4, 0, BR_FVECTOR3(1, 0, 0), BR_SCALAR(1)},    /*    1 */
    {{7, 6, 2}, {13, 6, 17}, NULL, 1, 4, 0, BR_FVECTOR3(0, 1, 0), BR_SCALAR(1)},  /*    9 */
    {{7, 2, 3}, {17, 0, 16}, NULL, 1, 1, 0, BR_FVECTOR3(0, 1, 0), BR_SCALAR(1)},  /*   10 */
    {{1, 5, 7}, {11, 14, 15}, NULL, 1, 4, 0, BR_FVECTOR3(0, 0, 1), BR_SCALAR(1)}, /*    7 */
    {{1, 7, 3}, {15, 16, 12}, NULL, 1, 1, 0, BR_FVECTOR3(0, 0, 1), BR_SCALAR(1)}, /*    8 */
    {{3, 0, 1}, {2, 8, 12}, NULL, 1, 1, 0, BR_FVECTOR3(-1, 0, 0), BR_SCALAR(1)},  /*    5 */
    {{3, 2, 0}, {0, 1, 2}, NULL, 1, 4, 0, BR_FVECTOR3(-1, 0, 0), BR_SCALAR(1)},   /*    0 */
    {{1, 0, 4}, {8, 9, 10}, NULL, 1, 4, 0, BR_FVECTOR3(0, -1, 0), BR_SCALAR(1)},  /*    3 */
    {{1, 4, 5}, {10, 3, 11}, NULL, 1, 1, 0, BR_FVECTOR3(0, -1, 0), BR_SCALAR(1)}, /*    4 */
    {{0, 6, 4}, {7, 4, 9}, NULL, 1, 1, 0, BR_FVECTOR3(0, 0, -1), BR_SCALAR(1)},   /*   11 */
    {{0, 2, 6}, {1, 6, 7}, NULL, 1, 4, 0, BR_FVECTOR3(0, 0, -1), BR_SCALAR(1)},   /*    2 */
};

/*
 * 1 Face Group
 */
static struct br_face_group zb_bounds_face_groups[] = {{NULL, zb_bounds_faces, 12}};

/*
 * 1 Vertex Group
 */
static struct br_vertex_group zb_bounds_vertex_groups[] = {{NULL, zb_bounds_vertices, 8}};

static struct br_model zb_bounds_model = {
    "zb_bounds",
    zb_bounds_vertices,
    zb_bounds_faces,

    8,
    12, /* nV nF */

    BR_VECTOR3(0, 0, 0), /* pivot */

    0, /* Flags */

    NULL,
    NULL, /* Custom, user */

    BR_SCALAR(1.73205), /* radius */

    {
        BR_VECTOR3(-1, -1, -1), /* min */
        BR_VECTOR3(1, 1, 1),    /* max */
    },

    8,  /* nprepared_vertices */
    12, /* nprepared_faces */

    zb_bounds_faces,
    zb_bounds_vertices,

    1, /* nface_groups */
    1, /* nvertex_groups */

    zb_bounds_face_groups,
    zb_bounds_vertex_groups,

    18, /* nEdges */

    NULL,
    NULL,
};

/*
 * Fills in above cuboid mesh that represents the given bounding box
 */
static br_model *ZbMakeMeshFromBounds(br_bounds *b)
{
    int i;

    /*
     * Fill in vertices
     */
    BrVector3Set(&zb_bounds_vertices[0].p, b->min.v[X], b->min.v[Y], b->min.v[Z]);
    BrVector3Set(&zb_bounds_vertices[1].p, b->min.v[X], b->min.v[Y], b->max.v[Z]);
    BrVector3Set(&zb_bounds_vertices[2].p, b->min.v[X], b->max.v[Y], b->min.v[Z]);
    BrVector3Set(&zb_bounds_vertices[3].p, b->min.v[X], b->max.v[Y], b->max.v[Z]);
    BrVector3Set(&zb_bounds_vertices[4].p, b->max.v[X], b->min.v[Y], b->min.v[Z]);
    BrVector3Set(&zb_bounds_vertices[5].p, b->max.v[X], b->min.v[Y], b->max.v[Z]);
    BrVector3Set(&zb_bounds_vertices[6].p, b->max.v[X], b->max.v[Y], b->min.v[Z]);
    BrVector3Set(&zb_bounds_vertices[7].p, b->max.v[X], b->max.v[Y], b->max.v[Z]);

    /*
     * Fill in plane equations of faces
     */
    for (i = 0; i < 3; i++)
    {
        zb_bounds_faces[i * 2].d = zb_bounds_faces[1 + i * 2].d = b->max.v[i];
        zb_bounds_faces[6 + i * 2].d = zb_bounds_faces[7 + i * 2].d = -b->min.v[i];
    }

    /*
     * Fill in bounds
     */
    zb_bounds_model.bounds = *b;

    return &zb_bounds_model;
}

/*
 * Render bounding box points
 */
void ZbBoundingBoxRenderPoints(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen)
{
    ZbMeshRenderPoints(actor, ZbMakeMeshFromBounds(&model->bounds), material, style, on_screen);
}

/*
 * Render bounding box edges
 */
void ZbBoundingBoxRenderEdges(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen)
{
    ZbMeshRenderEdges(actor, ZbMakeMeshFromBounds(&model->bounds), material, style, on_screen);
}

/*
 * Render bounding box faces
 */
void ZbBoundingBoxRenderFaces(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen)
{
    ZbMeshRender(actor, ZbMakeMeshFromBounds(&model->bounds), material, style, on_screen);
}

#if 0
/*
 * Debug rendering for zbmesh.c
 */
void GfxClipLine(int a1, int b1, int a2, int b2, int colour);

/*
 * Data structures used to hold bounding box - vertices are filled in from
 * current model
 */
static struct bbox_vertex {
	br_vector3 m;
	br_vector4 s;
} bbox_vertices[8];

static struct bbox_edge {
	int start;
	int end;
	int count;
} bbox_edges[] = {
	{0,1},		/* 0  */
	{1,3},		/* 1  */
	{2,3},		/* 2  */
	{0,2},		/* 3  */
	{4,5},		/* 4  */
	{5,7},		/* 5  */
	{6,7},		/* 6  */
	{4,6},		/* 7  */
	{1,5},		/* 8  */
	{3,7},		/* 9  */
	{2,6},		/* 10 */
	{0,4},		/* 11 */
};

static struct bbox_face {
	int vertices[4];
	int edges[4];
	br_vector3 n;
	br_scalar d;
	int visible;
} bbox_faces[] = {
	{{0,2,3,1},{ 3, 2, 1, 0},BR_VECTOR3( 0, 0,-1)},		/* 0 */
	{{0,4,6,2},{11, 7,10, 3},BR_VECTOR3(-1, 0, 0)},		/* 1 */
	{{0,1,5,4},{ 0, 8, 4,11},BR_VECTOR3( 0,-1, 0)},		/* 2 */
	{{7,6,4,5},{ 7, 4, 5, 6},BR_VECTOR3( 0, 0, 1)},		/* 3 */
	{{7,3,2,6},{ 2,10, 6, 9},BR_VECTOR3( 0, 1, 0)},		/* 4 */
	{{7,5,1,3},{ 5, 8, 1, 9},BR_VECTOR3( 1, 0, 0)},		/* 5 */
};


void StartBoundingBox(int on_screen)
{
	int colour = (on_screen==OSC_ACCEPT)?192+25:64+25;
	int i,j;
	struct bbox_face *fp;
	struct bbox_edge *ep;
	struct bbox_vertex *vp;

	/*
	 * Clear edge counts
	 */
	for(i=0; i< BR_ASIZE(bbox_edges); i++)
		bbox_edges[i].count = 0;

	/*
	 * Fill in vertices from model
	 */
	for(i=0,vp = bbox_vertices; i<8; i++, vp++) {
		/*
		 * Work out vertex from bounds
		 */
		vp->m.v[X] = (i & 1)?zb.model->bounds.max.v[X]:zb.model->bounds.min.v[X];
		vp->m.v[Y] = (i & 2)?zb.model->bounds.max.v[Y]:zb.model->bounds.min.v[Y];
		vp->m.v[Z] = (i & 4)?zb.model->bounds.max.v[Z]:zb.model->bounds.min.v[Z];
		
		/*
		 * transform and project
		 */	
		BrMatrix4ApplyP(&vp->s,&vp->m,&fw.model_to_screen);

		if(vp->s.v[W]) {
#if 0
			if(on_screen == OSC_ACCEPT) {
				vp->m.v[X] = fw.vp_ox + BR_DIV(vp->s.v[X],vp->s.v[W]);
				vp->m.v[Y] = fw.vp_oy + BR_DIV(vp->s.v[Y],vp->s.v[W]);
				vp->m.v[Z] = BR_MULDIV(-BR_SCALAR(0x7fff),vp->s.v[Z],vp->s.v[W]);
			} else
#endif
			{
				vp->m.v[X] = fw.vp_ox + BR_MULDIV(fw.vp_width, vp->s.v[X],vp->s.v[W]);
				vp->m.v[Y] = fw.vp_oy + BR_MULDIV(fw.vp_height,vp->s.v[Y],vp->s.v[W]);
				vp->m.v[Z] = BR_MULDIV(-BR_SCALAR(0x7fff),vp->s.v[Z],vp->s.v[W]);
			}
		}
	}

	/*
	 * Fill in faces from model
	 */
	bbox_faces[0].d = -zb.model->bounds.min.v[Z];
	bbox_faces[1].d = -zb.model->bounds.min.v[X];
	bbox_faces[2].d = -zb.model->bounds.min.v[Y];
	bbox_faces[3].d =  zb.model->bounds.max.v[Z];
	bbox_faces[4].d =  zb.model->bounds.max.v[Y];
	bbox_faces[5].d =  zb.model->bounds.max.v[X];

	/*
	 * See which faces are visible
	 */
	for(i=0, fp = bbox_faces; i< BR_ASIZE(bbox_faces); i++, fp++) {
		if(BrVector3Dot(&fp->n,&fw.eye_m) <= fp->d) {
			fp->visible = 0;
			continue;
		}

		fp->visible = 1;

		for(j=0; j< 4; j++)
			bbox_edges[fp->edges[j]].count++;
	}

	/*
	 * Plot hidden edges
	 */
	for(i=0, ep = bbox_edges; i< BR_ASIZE(bbox_edges); i++, ep++)
		if(ep->count == 0)
			GfxClipLine(
				BrScalarToInt(bbox_vertices[ep->start].m.v[X]+BR_SCALAR(0.5)),
				BrScalarToInt(bbox_vertices[ep->start].m.v[Y]+BR_SCALAR(0.5)),
				BrScalarToInt(bbox_vertices[ep->end].m.v[X]+BR_SCALAR(0.5)),
				BrScalarToInt(bbox_vertices[ep->end].m.v[Y]+BR_SCALAR(0.5)),
				colour);
}

void EndBoundingBox(int on_screen)
{
	int colour = (on_screen==OSC_ACCEPT)?192+54:64+54;
	int i;
	struct bbox_edge *ep;

	/*
	 * Plot visible edges
	 */
	for(i=0, ep = bbox_edges; i< BR_ASIZE(bbox_edges); i++, ep++)
		if(ep->count)
			GfxClipLine(
				BrScalarToInt(bbox_vertices[ep->start].m.v[X]+BR_SCALAR(0.5)),
				BrScalarToInt(bbox_vertices[ep->start].m.v[Y]+BR_SCALAR(0.5)),
				BrScalarToInt(bbox_vertices[ep->end].m.v[X]  +BR_SCALAR(0.5)),
				BrScalarToInt(bbox_vertices[ep->end].m.v[Y]  +BR_SCALAR(0.5)),
				colour);
}
#endif

