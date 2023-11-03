/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: zb.h 1.44 1995/08/31 16:47:50 sam Exp $
 * $Locker:  $
 *
 * Internal types and structures for z-buffer renderer
 */
#ifndef _ZB_H_
#define _ZB_H_

/*
 * Pull in all the framework and public definitions
 */
#ifndef _FW_H_
#include "fw.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define USER_CLIP 1

/*
 * Type for the on-screen vertex components
 */
#define SCREEN_FIXED 1

#if SCREEN_FIXED
    typedef br_fixed_ls screen_scalar;

#define ScreenToInt(x) BrFixedToInt(x)
#define IntToScreen(x) BrIntToFixed(x)

#define ScalarToScreen(x) BrScalarToFixed(x)
#define ScreenToScalar(x) BrFixedToScalar(x)

#define FixedToScreen(x) (x)
#define ScreenToFixed(x) (x)
#else

typedef int screen_scalar;

#define ScreenToInt(x) (x)
#define IntToScreen(x) (x)

#define ScalarToScreen(x) BrScalarToInt(x)
#define ScreenToScalar(x) BrIntToScalar(x)

#define FixedToScreen(x) BrFixedToInt(x)
#define ScreenToFixed(x) BrIntToFixed(x)

#endif

    /*
     * Per vertex (in model) structure
     *
     * Some code relies on this structure being 64 bytes long
     */
    struct temp_vertex
    {
        screen_scalar v[3]; /* Projected screen space point		*/
        br_uint_32 outcode; /* Outcodes for this vertex			*/
        br_uint_32 direction;
        br_scalar comp[NUM_COMPONENTS]; /* Shading components for vertex	*/
    };

    struct temp_vertex_fixed
    {
        screen_scalar v[3]; /* Projected screen space point		*/
        br_uint_32 outcode; /* Outcodes for this vertex			*/
        br_uint_32 direction;
        br_fixed_ls comp[NUM_COMPONENTS]; /* Shading components for vertex	*/
    };

    /*
     * Per edge (in model) structure
     */
    struct temp_edge
    {
        /*
         * Vertices at either end of edge
         */
        br_uint_16 vertices[2];
        br_uint_16 codes;

        br_uint_8 flag;
        br_uint_8 _pad[1];

        //	br_uint_32 surface;

        /*
         * Edge material
         */
        br_material *material;
    };

    /*
     * Per face (in model) structure
     */
    struct temp_face
    {
        /*
         * Result of face surface call
         *
         * Will either be an index or a true colour
         */
        br_uint_32 surface;

        /*
         * Combined outcodes of face
         */
        br_uint_16 codes;

        /*
         * Flag describing visiblity of face
         */
        br_uint_8 flag;
        br_uint_8 _pad[1];
    };

/*
 * Values for temp_face.flag
 */
#define TFF_VISIBLE 4
#define TFF_CLIPPED 2
#define TFF_REVERSED 1

/*
 * Values for temp_vertex.direction
 */
#define TVDIR_FRONT 0x01
#define TVDIR_BACK 0x02

    struct clip_vertex
    {

        br_uint_16 outcode;
        br_uint_16 base_vertex;

        /*
         * Components of vertex
         */
        br_scalar comp[NUM_COMPONENTS];
    };

    /*
     * Scan convertion details for one edge of triangle
     */
    struct scan_edge
    {
        br_fixed_ls x;    /* delta x along edge				*/
        br_fixed_ls y;    /* delta y along edge				*/
        br_fixed_ls grad; /* gradient (x/y)					*/
        br_int_32 f;      /* Starting value		(fraction)  */
        br_int_32 i;      /*						(integer)   */
        br_int_32 d_f;    /* delta_x per scanline	(fraction)  */
        br_int_32 d_i;    /*             			(integer)   */
        br_int_32 start;  /* starting scanline				*/
        br_int_32 count;  /* total scanlines				    */
    };

    /*
     * Scan convertion details for one parameter
     */
    struct scan_parameter
    {
        br_fixed_ls currentpix; /* Parameter (16.16) value at pixel				*/
        br_fixed_ls current;    /* Parameter (16.16) value at start of scanline	*/
        br_fixed_ls d_carry;    /* Increment per scanline if carry from bit 15	*/
        br_fixed_ls d_nocarry;  /*   ""              ""      no carry   ""    	*/
        br_fixed_ls grad_x;     /* Gradient of parameter along X axis			*/
        br_fixed_ls grad_y;     /* Gradient of parameter along Y axis			*/
    };

    /*
     * Arbitrary width scan line data
     */
    struct arbitrary_width_scan
    {
        struct scan_edge *edge;

        br_uint_8 *start, *end;
        br_fixed_ls *zstart;
        br_uint_8 *source_current;

        short u_int_current;

        short pad;

        unsigned u_current, du, du_carry, du_nocarry;
        int du_int, du_int_nocarry, du_int_carry;

        unsigned v_current, dv, dv_carry, dv_nocarry;
        int dsource, dsource_carry, dsource_nocarry;

        br_uint_8 *texture_start;
        int texture_size, texture_stride, texture_width;
    };

    /*
     * Perspective texture mapper data
     */
    struct perspective_scan
    {
        char *start, *end;
        char *zstart;
        int source;
        int y;
    };

    /*
     * Function types for primitives
     */
    typedef void BR_ASM_CALL br_triangle_fn(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                            struct temp_vertex_fixed *v2);
    typedef void BR_ASM_CALL br_line_fn(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);
    typedef void BR_ASM_CALL br_point_fn(struct temp_vertex_fixed *v0);

    /*
     * Callback functions for generic trapezoid and pixel plot fns.
     */
    typedef void BR_ASM_CALL br_trapezoid_render_cb(struct scan_edge *minor);
    typedef void BR_ASM_CALL br_pixel_render_cb(br_int_32 x, br_int_32 y);

    /*
     * Private state of renderer
     */
    typedef struct br_zbuffer_state
    {
        /*
         * Local copy of model -> screen with viewport factored in
         */
        br_matrix4 os_model_to_screen;
        br_scalar os_model_to_screen_hi[4];

        /*
         * Pointers into scratch area
         */
        br_int_8 *vertex_counts;
        br_int_32 *face_group_counts;
        br_int_8 *face_group_clipped;
        br_int_8 *edge_counts;
        struct temp_face *temp_faces;
        struct temp_edge *temp_edges;
        struct temp_vertex *temp_vertices;

        /*
         * Flag to indicate that vertex directions
         * have been initialised
         */
        int directions_cleared;

        /*
         * Model and material being rendered
         */
        br_model *model;
        br_material *material;

        /*
         * Current default material
         */
        br_material *default_material;

        /*
         * Misc. constant parameters for rendering
         */
        br_uint_8 *colour_buffer;  /* Colour buffer		*/
        br_fixed_ls *depth_buffer; /* Z buffer				*/
        br_int_32 row_width;       /* Stride (in bytes)	*/
        br_int_32 depth_row_width; /* Stride (in bytes)	*/

        br_uint_8 *texture_buffer; /* Texture map to use				*/
        br_uint_8 *bump_buffer;    /* Bump map to use					*/
        br_uint_8 *shade_table;    /* Indirection table				*/
        br_uint_8 *blend_table;    /* Indirection table				*/
        br_uint_8 *lighting_table; /* Lighting for quantised normals	*/
        br_uint_32 *screen_table;  /* Screen							*/

#if 0
	/*
	 * Table of x * row_width
	 */
	br_uint_32 row_table[MAX_OUTPUT_HEIGHT];
#endif

        /*
         * Workspace for triangle scan converter
         */
        struct scan_edge main; /* Long edge of triangle				   */
        struct scan_edge top;  /* Top short edge						   */
        struct scan_edge bot;  /* Bottom short edge					   */

        struct scan_parameter pz; /* Depth				*/

        struct scan_parameter pu; /* Mapping				*/
        struct scan_parameter pv;

        struct scan_parameter pi; /* Index				*/

        struct scan_parameter pr; /* Red					*/
        struct scan_parameter pg; /* Green				*/
        struct scan_parameter pb; /* Blue					*/

        struct scan_parameter pq; /* Perspective divisor	*/

        struct scan_parameter source; /* Current texel */

        /*
         * Per-component mask for scan converter interpolation
         */
        br_uint_32 component_mask; /* Components to interpolate */
        br_uint_32 correct_mask;   /* Components to apply perspective correction */

        br_uint_32 clip_mask;    /* Components to clip */
        br_uint_32 convert_mask; /* Components to convert scalar->float */

        /*
         * Global arbitrary width scan line data
         */
        struct arbitrary_width_scan awsl;

        /*
         * Perspective texture mapper globals
         */
        struct perspective_scan tsl;

        /*
         * Current triangle functions
         */
        br_triangle_fn *triangle_render;

        /*
         * Generic triangle rendering sub-functions
         */
        br_trapezoid_render_cb *trapezoid_render;
        br_pixel_render_cb *pixel_render;

        /*
         * Bounds of rendered actor
         */
        screen_scalar bounds[4];

        /*
         * Callback for visible actors
         */
        br_renderbounds_cbfn *bounds_call;

        /*
         * Current output type
         */
        struct zb_render_type *type;

        /*
         * Anchor resource for ZB renderer
         */
        void *res;

    } br_zbuffer_state;

    extern br_zbuffer_state BR_ASM_DATA zb;

    /*
     * Transform into screen space - Inline expanded BrMatrix4ApplyP()
     */
#define TRANSFORM_VERTEX_OS()                                                                                          \
    {                                                                                                                  \
        screen.v[X] = BR_MAC3(vp->p.v[X], fw.model_to_screen.m[0][0], vp->p.v[Y], fw.model_to_screen.m[1][0],          \
                              vp->p.v[Z], fw.model_to_screen.m[2][0]) +                                                \
                      fw.model_to_screen.m[3][0];                                                                      \
                                                                                                                       \
        screen.v[Y] = BR_MAC3(vp->p.v[X], fw.model_to_screen.m[0][1], vp->p.v[Y], fw.model_to_screen.m[1][1],          \
                              vp->p.v[Z], fw.model_to_screen.m[2][1]) +                                                \
                      fw.model_to_screen.m[3][1];                                                                      \
                                                                                                                       \
        screen.v[Z] = BR_MAC3(vp->p.v[X], fw.model_to_screen.m[0][2], vp->p.v[Y], fw.model_to_screen.m[1][2],          \
                              vp->p.v[Z], fw.model_to_screen.m[2][2]) +                                                \
                      fw.model_to_screen.m[3][2];                                                                      \
                                                                                                                       \
        screen.v[W] = BR_MAC3(vp->p.v[X], fw.model_to_screen.m[0][3], vp->p.v[Y], fw.model_to_screen.m[1][3],          \
                              vp->p.v[Z], fw.model_to_screen.m[2][3]) +                                                \
                      fw.model_to_screen.m[3][3];                                                                      \
    }

/*
 * Generic macro for projecting a vertex from homogenous coordinates
 */
#define PROJECT_VERTEX(tvp, sx, sy, sz, sw)                                                                            \
    {                                                                                                                  \
        (tvp)->v[X] = ScalarToScreen(fw.vp_ox + BR_MULDIV(fw.vp_width, (sx), (sw)));                                   \
        (tvp)->v[Y] = ScalarToScreen(fw.vp_oy + BR_MULDIV(fw.vp_height, (sy), (sw)));                                  \
        (tvp)->v[Z] = BrScalarToFixed(PERSP_DIV_Z((sz), (sw)));                                                        \
    }

/*
 * Alignment for block in scratch area
 */
#define SCRATCH_BOUNDARY 16
#define SCRATCH_ALIGN(x) (((x) + (SCRATCH_BOUNDARY - 1)) & ~(SCRATCH_BOUNDARY - 1))

/*
 * New line and point clamps
 */
#define CLAMP_LP(x, y)                                                                                                 \
    {                                                                                                                  \
        if (x < 0)                                                                                                     \
            x = 0;                                                                                                     \
        if (y < 0)                                                                                                     \
            y = 0;                                                                                                     \
        if (x >= fw.output->width)                                                                                     \
            x--;                                                                                                       \
        if (y >= fw.output->height)                                                                                    \
            y--;                                                                                                       \
    }

/*
 * Include bounding rectangle callback
 */
#define BOUNDING_RECTANGLE_CALL 1

#if BOUNDING_RECTANGLE_CALL
/*
 * Update MIN and MAX vertices' x,y
 */
#define UPDATE_BOUNDS(tv)                                                                                              \
    {                                                                                                                  \
        if ((tv).v[X] > zb.bounds[BR_BOUNDS_MAX_X])                                                                    \
            zb.bounds[BR_BOUNDS_MAX_X] = (tv).v[X];                                                                    \
        if ((tv).v[X] < zb.bounds[BR_BOUNDS_MIN_X])                                                                    \
            zb.bounds[BR_BOUNDS_MIN_X] = (tv).v[X];                                                                    \
        if ((tv).v[Y] > zb.bounds[BR_BOUNDS_MAX_Y])                                                                    \
            zb.bounds[BR_BOUNDS_MAX_Y] = (tv).v[Y];                                                                    \
        if ((tv).v[Y] < zb.bounds[BR_BOUNDS_MIN_Y])                                                                    \
            zb.bounds[BR_BOUNDS_MIN_Y] = (tv).v[Y];                                                                    \
    }

#define CLAMP_POINT(x, y)                                                                                              \
    {                                                                                                                  \
        if ((x) <= ScalarToScreen(fw.vp_ox - fw.vp_width))                                                             \
            (x) = ScalarToScreen(fw.vp_ox - fw.vp_width) + 1;                                                          \
        if ((x) >= ScalarToScreen(fw.vp_ox + fw.vp_width))                                                             \
            (x) = ScalarToScreen(fw.vp_ox + fw.vp_width) - 1;                                                          \
                                                                                                                       \
        if ((y) <= ScalarToScreen(fw.vp_oy + fw.vp_height))                                                            \
            (y) = ScalarToScreen(fw.vp_oy + fw.vp_height) + 1;                                                         \
        if ((y) >= ScalarToScreen(fw.vp_oy - fw.vp_height))                                                            \
            (y) = ScalarToScreen(fw.vp_oy - fw.vp_height) - 1;                                                         \
    }

#define CLAMP_POINT_MIN(x, y)                                                                                          \
    {                                                                                                                  \
        if ((x) <= ScalarToScreen(fw.vp_ox - fw.vp_width))                                                             \
            (x) = ScalarToScreen(fw.vp_ox - fw.vp_width) + 1;                                                          \
                                                                                                                       \
        if ((y) <= ScalarToScreen(fw.vp_oy + fw.vp_height))                                                            \
            (y) = ScalarToScreen(fw.vp_oy + fw.vp_height) + 1;                                                         \
    }

#define CLAMP_POINT_MAX(x, y)                                                                                          \
    {                                                                                                                  \
        if ((x) >= ScalarToScreen(fw.vp_ox + fw.vp_width))                                                             \
            (x) = ScalarToScreen(fw.vp_ox + fw.vp_width) - 1;                                                          \
                                                                                                                       \
        if ((y) >= ScalarToScreen(fw.vp_oy - fw.vp_height))                                                            \
            (y) = ScalarToScreen(fw.vp_oy - fw.vp_height) - 1;                                                         \
    }

#else
#define UPDATE_BOUNDS(tv)
#endif

    typedef void BR_ASM_CALL br_face_group_fn(struct br_face_group *gp, struct temp_face *tfp);

    /*
     * Structure describing one particular sort of material renderer
     */
    struct zb_material_type
    {
        /*
         * Descriptive string
         */
        char *identifier;

        /*
         * If
         *		mat->flags & flags_mask) == flags_cmp
         * 	    colour_map->map_type == map_type
         *		width != 0 && colour_map->width == width
         *		height != 0 && colour_map->height == height
         *
         *  material matches this type
         */
        br_uint_32 flags_mask;
        br_uint_32 flags_cmp;
        br_uint_8 map_type;
        br_uint_16 width;
        br_uint_16 height;

        br_face_group_fn *face_group;
        br_triangle_fn *triangle;

        br_line_fn *line;
        br_point_fn *point;

        br_uint_32 clip_mask;
        br_uint_32 convert_mask;
    };

/*
 * Flags used to augment material flags when specifiying
 * material type
 */
#define ZB_MATF_HAS_MAP 0x80000000
#define ZB_MATF_TRANSPARENT 0x40000000
#define ZB_MATF_HAS_SCREEN 0x20000000
#define ZB_MATF_SQUARE_POW2 0x10000000
#define ZB_MATF_NO_SKIP 0x08000000
#define ZB_MATF_MAP_OPACITY 0x04000000
#define ZB_MATF_MAP_TRANSPARENT 0x02000000
#define ZB_MATF_HAS_SHADE 0x01000000

    /*
     * Structure describing one particular class of renderer
     */
    struct zb_render_type
    {
        /*
         * Descriptive string
         */
        char *identifier;

        /*
         * Type of colour buffer
         */
        br_uint_8 colour_type;

        /*
         * Type of depth buffer
         */
        br_uint_8 depth_type;

        br_uint_16 user_type; /* XXX unused at the moment */

        /*
         * Used to check if the row_width's of the two pixelmaps
         * match -
         *	if colour row_width * depth_row_size == depth row_width * colour_row_size
         *	then this set of functions match
         */
        br_uint_8 colour_row_size;
        br_uint_8 depth_row_size;

        /*
         * Flag to indicate true-colour
         */
        br_uint_8 true_colour;

        /*
         * A table of supported material types
         */
        struct zb_material_type *material_types;

        int nmaterial_types;
    };

#ifdef __cplusplus
};
#endif

/*
 * Pull in private prototypes
 */
#ifndef _NO_PROTOTYPES

#ifndef _ZBIPROTO_H_
#include "zbiproto.h"
#endif

#ifndef _FWIPROTO_H_
#include "fwiproto.h"
#endif

#endif

#endif


