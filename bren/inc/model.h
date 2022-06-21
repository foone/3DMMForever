/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: model.h 1.20 1995/08/31 16:36:18 sam Exp $
 * $Locker:  $
 *
 * In-memory structures for models, both public and private areas
 */
#ifndef _MODEL_H_
#define _MODEL_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct br_vertex
    {
        br_vector3 p;   /* Point in model space		*/
        br_vector2 map; /* Mapping coordinates		*/

        /*
         * Index for prelit models
         */
        br_uint_8 index;

        /*
         * true colour for pre-lit models
         */
        br_uint_8 red;
        br_uint_8 grn;
        br_uint_8 blu;

        /*
         * Private fields
         */
        br_uint_16 r;
        br_fvector3 n; /* Surface normal at vertex	*/

    } br_vertex;

    typedef struct br_face
    {
        br_uint_16 vertices[3]; /* Vertices around face 				*/
        br_uint_16 edges[3];    /* Edges around face					*/
        br_material *material;  /* Face material (or NULL) 				*/
        br_uint_16 smoothing;   /* Controls if shared edges are smooth	*/
        br_uint_8 flags;        /* Bits 0,1 and 2 denote internal edges	*/
        br_uint_8 _pad0;
        br_fvector3 n; /* Plane equation of face				*/
        br_scalar d;
    } br_face;

    /*
     * Bits for face flags
     */
    enum
    {
        BR_FACEF_COPLANAR_0 = 0x01, /* The face adjoining edge 0 is coplanar with this face */
        BR_FACEF_COPLANAR_1 = 0x02, /*         ""              1          ""                */
        BR_FACEF_COPLANAR_2 = 0x04  /*         ""              1          ""                */
    };

    typedef struct br_face_group
    {
        br_material *material; /* Group material (or NULL) 			*/
        br_face *faces;        /* faces in group						*/
        br_uint_16 nfaces;     /* Number of faces in a group			*/
    } br_face_group;

    typedef struct br_vertex_group
    {
        br_material *material; /* Group material (or NULL) 			*/
        br_vertex *vertices;   /* vertices in group					*/
        br_uint_16 nvertices;  /* Number of vertices in a group		*/
    } br_vertex_group;

    /*
     * Callback function type for custom models
     */
    struct br_actor;
    struct br_model;
    struct br_material;

    typedef void BR_CALLBACK br_model_custom_cbfn(struct br_actor *actor, struct br_model *model,
                                                  struct br_material *material, void *render_data, br_uint_8 style,
                                                  int on_screen, br_matrix34 *model_to_view,
                                                  br_matrix4 *model_to_screen);

    typedef struct br_model
    {
        char *identifier;

        br_vertex *vertices;
        br_face *faces;

        br_uint_16 nvertices;
        br_uint_16 nfaces;

        /*
         * Offset of model's pivot point (where it attaches to parent)
         */
        br_vector3 pivot;

        /*
         * Flags describing what is allowed in ModelPrepare()
         */
        br_uint_16 flags;

        /*
         * Application call
         */
        br_model_custom_cbfn *custom;

        /*
         * Application defined data - untouched by system
         */
        void *user;

        /*
         * Generated by ModelUpdate
         */
        /*
         * Bounding radius of model from origin
         */
        br_scalar radius;

        /*
         * Axis-aligned box that bound model in model coords
         */
        br_bounds bounds;

        /*
         * Vertices and faces that have been sorted
         * into groups, removing conflicts at material boundaries
         * and smoothign groups
         */
        br_uint_16 nprepared_vertices;
        br_uint_16 nprepared_faces;

        br_face *prepared_faces;
        br_vertex *prepared_vertices;

        /*
         * Groups of faces and vertices, by material
         */
        br_uint_16 nface_groups;
        br_uint_16 nvertex_groups;

        br_face_group *face_groups;
        br_vertex_group *vertex_groups;

        /*
         * Upper limit on the face->edges[] entries
         */
        br_uint_16 nedges;

        /*
         * Pointers to tables that map prepared face and vertex indexes
         * back to original face index
         *
         * Only generated if BR_MODF_GENERATE_TAGS is set
         */
        br_uint_16 *face_tags;
        br_uint_16 *vertex_tags;

        /*
         * Private fields
         */
        br_uint_32 prep_flags;
        br_uint_16 *smooth_strings;
        void *rptr;
    } br_model;

    /*
     * Bits for br_model->flags
     */
    enum
    {
        BR_MODF_DONT_WELD = 0x0001,     /* Vertices with same x,y,z cannot be merged	*/
        BR_MODF_KEEP_ORIGINAL = 0x0002, /* Don't release model->faces/vertices during ModelUpdate() */
        BR_MODF_GENERATE_TAGS = 0x0004, /* Allocate and fill in the face and vertex tag structures */
        BR_MODF_QUICK_UPDATE = 0x0010,  /* ModelUpdate is fast - but may produce slower models */

        BR_MODF_CUSTOM = 0x0020, /* Invoke custom callback for this model */

        BR_MODF_PREPREPARED = 0x0040 /* Model structure is pre-prepared - update performs no work */
    };

    /*
     * Flags to BrModelUpdate()
     */
    enum
    {
        BR_MODU_NORMALS = 0x0001,
        BR_MODU_EDGES = 0x0002,
        BR_MODU_RADIUS = 0x0004,
        BR_MODU_GROUPS = 0x0008,
        BR_MODU_BOUNDING_BOX = 0x0010,
        BR_MODU_MATERIALS = 0x0020,
        BR_MODU_ALL = 0x7fff
    };

/*
 * Backwards compatibility
 */
#define BR_MPREP_NORMALS BR_MODU_NORMALS
#define BR_MPREP_EDGES BR_MODU_EDGES
#define BR_MPREP_RADIUS BR_MODU_RADIUS
#define BR_MPREP_GROUPS BR_MODU_GROUPS
#define BR_MPREP_BOUNDING_BOX BR_MODU_BOUNDING_BOX
#define BR_MPREP_ALL BR_MODU_ALL

    /*
     * Values for BrModelApplyMap()
     */
    enum br_apply_map_types
    {
        BR_APPLYMAP_PLANE,
        BR_APPLYMAP_SPHERE,
        BR_APPLYMAP_CYLINDER,
        BR_APPLYMAP_DISC,
        BR_APPLYMAP_NONE
    };

    /*
     * Axis values for BrModelFitMap()
     */
    enum br_fitmap_axis
    {
        BR_FITMAP_PLUS_X,
        BR_FITMAP_PLUS_Y,
        BR_FITMAP_PLUS_Z,
        BR_FITMAP_MINUS_X,
        BR_FITMAP_MINUS_Y,
        BR_FITMAP_MINUS_Z
    };

#ifdef __cplusplus
};
#endif
#endif

/*
 * Local Variables:
 * tab-width: 4
 * End:
 */
