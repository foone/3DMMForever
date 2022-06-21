/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: fw.h 1.39 1995/07/28 19:01:47 sam Exp $
 * $Locker: sam $
 *
 * Internal types and structures for framework
 */
#ifndef _FW_H_
#define _FW_H_

/*
 * Pull in all the public definitions/declarations
 */
#ifndef _BRENDER_H_
#include "brender.h"
#endif

#ifndef _POOL_H_
#include "pool.h"
#endif

#ifndef _BRLISTS_H_
#include "brlists.h"
#endif

#ifndef _BREXCEPT_H_
#include "brexcept.h"
#endif

#ifndef _BRIMAGE_H_
#include "brimage.h"
#endif
#ifndef _REGISTER_H_
#include "register.h"
#endif

#ifndef _DEVICE_H_
#include "device.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * Per vertex components
     */
    enum
    {
        C_X,
        C_Y,
        C_Z,
        C_W,
        C_U,
        C_V,
        C_I,
        C_R,
        C_G,
        C_B,
        C_Q,
        NUM_COMPONENTS
    };

    /*
     * Mask bits for selecting which components take part in various operations
     */
    enum
    {
        CM_X = (1 << C_X),
        CM_Y = (1 << C_Y),
        CM_Z = (1 << C_Z),
        CM_W = (1 << C_W),
        CM_U = (1 << C_U),
        CM_V = (1 << C_V),
        CM_I = (1 << C_I),
        CM_R = (1 << C_R),
        CM_G = (1 << C_G),
        CM_B = (1 << C_B),
        CM_Q = (1 << C_Q)
    };

    /*
     * Limited set of components used for lighting sub functions
     */
    enum
    {
        L_I,
        L_R,
        L_G,
        L_B,
        NUM_L_COMPONENTS
    };

    /*
     * Private information about a light as it applies to the current model
     */
    typedef struct br_active_light
    {

        /*
         * Light intensity
         */
        br_scalar intensity;

        /*
         * position and direction of light in view space
         */
        br_vector3 view_position;
        br_vector3 view_direction;

        /*
         * Position and direction of light in final space, view or model
         */
        br_vector3 position;
        br_vector3 direction;

        /*
         * Half vector for specular effects
         */
        br_vector3 half;

        /*
         * Cosine of spot angles
         */
        br_scalar spot_cosine_outer;
        br_scalar spot_cosine_inner;

        /*
         * Function to evaluate this light
         */
        void (*light_sub_function)(br_vector3 *p, br_fvector3 *n, struct br_active_light *alp, br_scalar *lcomp);

        /*
         * Copy of light's type - may vary from original if a model's relationship
         * to light allows optimisation, eg:
         *  very far point light -> directional light
         *  model compeletely within spot cone -> point light
         */
        int type;

        br_light *light;

    } br_active_light;

    /*
     * Private information about an active clip plane
     */
    typedef struct br_active_clip_plane
    {
        /*
         * Planer eqn. in screen space
         */
        br_vector4 screen_plane;
    } br_active_clip_plane;

/*
 * Framework has a table of functions used for lighting - this
 * will be initialised according to what sort of output is required
 * (eg: indexed or RGB)
 */
#ifdef __WATCOMCX__
#define BR_SURFACE_CALL
#else
#define BR_SURFACE_CALL BR_ASM_CALL
#endif

    typedef void BR_SURFACE_CALL br_surface_fn(br_vertex *v, br_fvector3 *n, br_scalar *comp);
    typedef br_uint_32 BR_SURFACE_CALL br_face_surface_fn(br_vertex *v, br_face *fp, int reversed);

    typedef void br_light_sub_fn(br_vector3 *p, br_fvector3 *n, struct br_active_light *alp, br_scalar *lcomp);

    typedef void br_model_update_cbfn(br_model *model, br_uint_16 flags);
    typedef void br_material_update_cbfn(br_material *mat, br_uint_16 flags);
    typedef void br_table_update_cbfn(br_pixelmap *table, br_uint_16 flags);
    typedef void br_map_update_cbfn(br_pixelmap *map, br_uint_16 flags);

    struct fw_fn_table
    {

        /*
         * Top level surface functions
         */
        br_surface_fn *light;          /* Evauluate lighting	*/
        br_surface_fn *light_material; /* Copy material 		*/
        br_surface_fn *light_vertex;   /* Copy vertex 			*/

        br_surface_fn *light_texture; /* simple light plus simple texture mapping */

        br_face_surface_fn *face_light; /* Evauluate surface for a face	*/

        /*
         * Lighting
         */
        br_light_sub_fn *direct;
        br_light_sub_fn *point;
        br_light_sub_fn *point_attn;
        br_light_sub_fn *spot;
        br_light_sub_fn *spot_attn;
    };

    /*
     * Private state of framework
     */
    typedef struct br_framework_state
    {

        /*
         * Main surface properties function
         */
        br_surface_fn *surface_fn;

        /*
         * Called after U,V generation
         */
        br_surface_fn *surface_fn_after_map;

        /*
         * Called after component copying
         */
        br_surface_fn *surface_fn_after_copy;

        /*
         * Lighting a face - usually calls vertex function and converts result for faces
         */
        br_face_surface_fn *face_surface_fn;

        /*
         * Current mapping coordinate transform
         */
        br_matrix23 map_transform;

        /*
         * Base and scale for indexes
         */
        br_scalar index_base;
        br_scalar index_range;

        /*
         * Current Transforms
         */
        br_matrix4 model_to_screen;
        br_matrix4 view_to_screen;

        br_matrix34 model_to_view;
        br_matrix34 view_to_model;
        br_matrix34 model_to_environment;

        /*
         * List of transforms from camera to root with associated actor address
         */
        struct
        {
            br_matrix34 m;
            br_actor *a;
        } camera_path[MAX_CAMERA_DEPTH];

        /*
         * Type of view to screen transform
         */
        int vtos_type;

        /*
         * COP in model space
         */
        br_vector3 eye_m;
        br_vector3 eye_m_normalised;

        /*
         * Material curently in use - used for lighting calcs.
         */
        br_material *material;

        /*
         * Information about each active light, the lights that need to be
         * processed in model space are first, followed by those processed
         * in view space
         */
        br_active_light active_lights_model[BR_MAX_LIGHTS];
        br_active_light active_lights_view[BR_MAX_LIGHTS];

        /*
         * numbers of active lights of each type
         */
        br_uint_16 nactive_lights_model;
        br_uint_16 nactive_lights_view;

        /*
         * Flag that is set if there is only 1 direct light in model space
         */
        int light_is_1md;

        /*
         * Eye vector to use for lighting, either in model or view space
         */
        br_vector3 eye_l;

        /*
         * A table of active clip planes
         */
        br_active_clip_plane active_clip_planes[BR_MAX_CLIP_PLANES];
        br_uint_16 nactive_clip_planes;

        /*
         * A table of pointers to enabled lights
         *
         * If an entry is not NULL then the referenced actor is an enabled light
         */
        br_actor *enabled_lights[BR_MAX_LIGHTS];

        /*
         * A table of pointers to enabled clip planes
         *
         * If an entry is not NULL then the referenced actor is a clip plane
         */
        br_actor *enabled_clip_planes[BR_MAX_CLIP_PLANES];

        /*
         * A pointer to the current environment - if NULL, then the model's
         * local frame will be used
         */
        br_actor *enabled_environment;

        /*
         * Output pixelmap
         */
        br_pixelmap *output;

        /*
         * Viewport parameters
         */
        br_scalar vp_width, vp_height, vp_ox, vp_oy;

        /*
         * Flag that is true if rendering is going on
         */
        int rendering;

        /*
         * Various lists of registered items
         */
        br_registry reg_models;
        br_registry reg_materials;
        br_registry reg_textures;
        br_registry reg_tables;
        br_registry reg_resource_classes;

        /*
         * An index of registered resources by class
         */
        br_resource_class *resource_class_index[BR_MAX_RESOURCE_CLASSES];

        /*
         * Renderer functions for item preperation & update
         */
        br_model_update_cbfn *model_update;
        br_material_update_cbfn *material_update;
        br_table_update_cbfn *table_update;
        br_map_update_cbfn *map_update;

        /*
         * Current filesystem, memory, and error handlers
         */
        br_filesystem *fsys;
        br_allocator *mem;
        br_diaghandler *diag;

        /*
         * File write mode
         */
        int open_mode;

        /*
         * Base resource of which everything else is
         * a descendant
         */
        void *res;

        /*
         * Default model
         */
        br_model *default_model;

        /*
         * Default material
         */
        br_material *default_material;

        /*
         * Function pointers to current lighting ops.
         */
        struct fw_fn_table fn_table;

        /*
         * Global scratch space
         */
        void *scratch_ptr;
        br_size_t scratch_size;
        br_size_t scratch_last;
        int scratch_inuse;

    } br_framework_state;

    /*
     * Global renderer state
     */
    extern br_framework_state fw;

/*
 * Minimum scratch space to allocate for render temps.
 */
#define MIN_WORKSPACE 8192

/*
 * Make sure NULL is defined
 */
#ifndef NULL
#define NULL 0
#endif

    /*
     * Class of view->screen transform
     */
    enum view_to_screen_type
    {
        BR_VTOS_PARALLEL,
        BR_VTOS_PERSPECTIVE,
        BR_VTOS_4X4
    };

/*
 * Magic value for vertex->r to say that this vertex does not reuse a previous x,y,z
 */
#define NO_REUSE 0xffff

/*
 * The level below which the fixed specular power function is zero
 */
#define SPECULARPOW_CUTOFF BR_SCALAR(0.6172)

/*
 * Material flags generated during MaterialUpdate()
 */
#define MATUF_SURFACE_FACES 0x0001
#define MATUF_SURFACE_VERTICES 0x0002
#define MATUF_REGISTERED 0x0004

/*
 * Model flags generated during ModelUpdate()
 */
#define MODUF_SIMPLE_GROUPS 0x0001       /* Smoothing group is the same for the whole model */
#define MODUF_VERTEX_GROUPS_MATCH 0x0002 /* There are the same number of vertex groups, using the same materials */
#define MODUF_REGISTERED 0x0004
#define MODUF_HAS_PIVOT 0x0008 /* Model has non-zero pivot point */

/*
 * Perspective dive for Z component of vertices
 */
#if BASED_FIXED
#define PERSP_DIV_Z(a, b) (BrFixedDivF(-a, b))
#endif

#if BASED_FLOAT
#define PERSP_DIV_Z(a, b) (-32767.0 * (a) / (b))
#endif

/*
 * Generates outcodes for a homogenous point
 */
#define OUTCODE_POINT(outcode, screen)                                                                                 \
    {                                                                                                                  \
        int c;                                                                                                         \
                                                                                                                       \
        (outcode) = OUTCODES_NOT;                                                                                      \
        /*                                                                                                             \
         * The 6 planes of the view volume...                                                                          \
         */                                                                                                            \
        if ((screen)->v[X] > (screen)->v[W])                                                                           \
            (outcode) ^= (OUTCODE_LEFT | OUTCODE_N_LEFT);                                                              \
                                                                                                                       \
        if ((screen)->v[X] < -(screen)->v[W])                                                                          \
            (outcode) ^= (OUTCODE_RIGHT | OUTCODE_N_RIGHT);                                                            \
                                                                                                                       \
        if ((screen)->v[Y] > (screen)->v[W])                                                                           \
            (outcode) ^= (OUTCODE_TOP | OUTCODE_N_TOP);                                                                \
        if ((screen)->v[Y] < -(screen)->v[W])                                                                          \
            (outcode) ^= (OUTCODE_BOTTOM | OUTCODE_N_BOTTOM);                                                          \
                                                                                                                       \
        if (-(screen)->v[Z] > (screen)->v[W])                                                                          \
            (outcode) ^= (OUTCODE_YON | OUTCODE_N_YON);                                                                \
        if ((screen)->v[Z] > 0)                                                                                        \
            (outcode) ^= (OUTCODE_HITHER | OUTCODE_N_HITHER);                                                          \
                                                                                                                       \
        /*                                                                                                             \
         * Any user defined clip planes...                                                                             \
         */                                                                                                            \
        for (c = 0; c < fw.nactive_clip_planes; c++)                                                                   \
            if (BrVector4Dot((screen), &fw.active_clip_planes[c].screen_plane) < S0)                                   \
                outcode ^= (OUTCODE_USER | OUTCODE_N_USER) << c;                                                       \
    }

#if 1
#define APPLY_UV(du, dv, su, sv)                                                                                       \
    {                                                                                                                  \
        du = BR_ADD(BR_MAC2(fw.map_transform.m[0][0], su, fw.map_transform.m[1][0], sv), fw.map_transform.m[2][0]);    \
        dv = BR_ADD(BR_MAC2(fw.map_transform.m[0][1], su, fw.map_transform.m[1][1], sv), fw.map_transform.m[2][1]);    \
    }
#else
#define APPLY_UV(du, dv, su, sv)                                                                                       \
    {                                                                                                                  \
        du = BR_ADD(BR_MUL(fw.map_transform.m[0][0], su), fw.map_transform.m[2][0]);                                   \
        dv = BR_ADD(BR_MUL(fw.map_transform.m[1][1], sv), fw.map_transform.m[2][1]);                                   \
    }
#endif

#if DEBUG
/*
 * Controls whether the source file/line of each
 * resource allocation is tracked
 */
#define BR_RES_TRACKING 1

/*
 * True if resources are tagged with a magic number
 */
#define BR_RES_TAGGING 1
#else
#define BR_RES_TRACKING 1
#define BR_RES_TAGGING 1
#endif

/*
 * Pull in private prototypes
 */
#ifndef _NO_PROTOTYPES

#ifndef _FWIPROTO_H_
#include "fwiproto.h"
#endif

#ifndef _NO_VECTOR_MACROS
#ifndef _VECIFNS_H_
#include "vecifns.h"
#endif
#endif

#endif

#ifdef __cplusplus
};
#endif
#endif
