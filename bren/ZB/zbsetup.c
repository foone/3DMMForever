/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: zbsetup.c 1.18 1995/08/31 16:48:03 sam Exp $
 * $Locker:  $
 *
 */

//	 Hawkeye. Set all non-used types to the NULL render.

#include "zb.h"
#include <string.h>

static char rscid[] = "$Id: zbsetup.c 1.18 1995/08/31 16:48:03 sam Exp $";

/*
 * Global renderer state
 */
br_zbuffer_state BR_ASM_DATA zb;

void BR_ASM_CALL TriangleRender_Null(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                     struct temp_vertex_fixed *v2);
void BR_ASM_CALL LineRender_Null(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);
void BR_ASM_CALL PointRender_Null(struct temp_vertex_fixed *v0);

/*
 * Material types for INDEX_8, DEPTH_16
 */
#define MT_MASK (BR_MATF_SMOOTH | ZB_MATF_HAS_MAP | ZB_MATF_TRANSPARENT)

#define MT_MASK_T (BR_MATF_SMOOTH | BR_MATF_DECAL | ZB_MATF_HAS_MAP | ZB_MATF_TRANSPARENT)

#define MT_MASK_TP (BR_MATF_SMOOTH | BR_MATF_PERSPECTIVE | BR_MATF_DECAL | ZB_MATF_HAS_MAP | ZB_MATF_TRANSPARENT)

#define CM_COORDS (CM_X | CM_Y | CM_Z | CM_W)

/*
 * Only include identifiers in debug code
 */
#if DEBUG
#define IDENT(x) x
#else
#define IDENT(x) x
#endif

static struct zb_material_type mat_types_index_8[] = {

    //	/*
    //	 * Perspective texture mapping
    //	 */
    //	{
    //		IDENT("Texture mapped (perspective, dithered) 1024x1024"),
    //		MT_MASK_TP | ZB_MATF_NO_SKIP | BR_MATF_LIGHT | BR_MATF_PRELIT | BR_MATF_DITHER, ZB_MATF_HAS_MAP |
    //ZB_MATF_NO_SKIP | BR_MATF_DITHER | BR_MATF_PERSPECTIVE,
    //		BR_PMT_INDEX_8, 1024, 1024,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2TPD1024, LineRenderPIZ2T, PointRenderPIZ2T,
    //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V, CM_W | CM_U | CM_V,
    //	},
    //	{
    //		IDENT("Texture mapped (perspective) 1024x1024"),
    //		MT_MASK_TP | ZB_MATF_NO_SKIP | BR_MATF_LIGHT | BR_MATF_PRELIT, ZB_MATF_HAS_MAP | ZB_MATF_NO_SKIP |
    //BR_MATF_PERSPECTIVE,
    //		BR_PMT_INDEX_8, 1024, 1024,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2TP1024, LineRenderPIZ2T, PointRenderPIZ2T,
    //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V, CM_W | CM_U | CM_V,
    //	},
    //	{
    //		IDENT("Texture mapped (perspective) 256x256"),
    //		MT_MASK_TP | ZB_MATF_NO_SKIP | BR_MATF_LIGHT | BR_MATF_PRELIT, ZB_MATF_HAS_MAP | ZB_MATF_NO_SKIP |
    //BR_MATF_PERSPECTIVE,
    //		BR_PMT_INDEX_8, 256, 256,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2TP256, LineRenderPIZ2T, PointRenderPIZ2T,
    //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V, CM_W | CM_U | CM_V,
    //	},
    //	{
    //		IDENT("Texture mapped (perspective) 64x64"),
    //		MT_MASK_TP | ZB_MATF_NO_SKIP | BR_MATF_LIGHT | BR_MATF_PRELIT, ZB_MATF_HAS_MAP | ZB_MATF_NO_SKIP |
    //BR_MATF_PERSPECTIVE,
    //		BR_PMT_INDEX_8, 64, 64,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2TP64, LineRenderPIZ2T, PointRenderPIZ2T,
    //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V, CM_W | CM_U | CM_V,
    //	},
    //	{
    //		IDENT("Lit smooth texture mapped (perspective) 1024x1024"),
    //		MT_MASK_TP | ZB_MATF_NO_SKIP | BR_MATF_LIGHT, BR_MATF_SMOOTH | BR_MATF_LIGHT | ZB_MATF_HAS_MAP |
    //ZB_MATF_NO_SKIP | BR_MATF_PERSPECTIVE,
    //		BR_PMT_INDEX_8, 1024, 1024,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2TIP1024, LineRenderPIZ2TI, PointRenderPIZ2TI,
    //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V | CM_I, CM_W | CM_U | CM_V | CM_I,
    //	},
    //	{
    //		IDENT("Lit smooth texture mapped (perspective) 256x256"),
    //		MT_MASK_TP | ZB_MATF_NO_SKIP | BR_MATF_LIGHT, BR_MATF_SMOOTH | BR_MATF_LIGHT | ZB_MATF_HAS_MAP |
    //ZB_MATF_NO_SKIP | BR_MATF_PERSPECTIVE,
    //		BR_PMT_INDEX_8, 256, 256,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2TIP256, LineRenderPIZ2TI, PointRenderPIZ2TI,
    //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V | CM_I, CM_W | CM_U | CM_V | CM_I,
    //	},
    //	{
    //		IDENT("Lit smooth texture mapped (perspective) 64x64"),
    //		MT_MASK_TP | ZB_MATF_NO_SKIP | BR_MATF_LIGHT, BR_MATF_SMOOTH | BR_MATF_LIGHT | ZB_MATF_HAS_MAP |
    //ZB_MATF_NO_SKIP | BR_MATF_PERSPECTIVE,
    //		BR_PMT_INDEX_8, 64, 64,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2TIP64, LineRenderPIZ2TI, PointRenderPIZ2TI,
    //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V | CM_I, CM_W | CM_U | CM_V | CM_I,
    //	},
    //
    //	/*
    //	 * Special case 256x256 linear texture mapping
    //	 */
    //	{
    //		IDENT("Texture mapped (linear, dithered) 256x256"),
    //		MT_MASK_TP | ZB_MATF_NO_SKIP | BR_MATF_LIGHT | BR_MATF_PRELIT | BR_MATF_DITHER, ZB_MATF_HAS_MAP |
    //ZB_MATF_NO_SKIP | BR_MATF_DITHER,
    //		BR_PMT_INDEX_8, 256, 256,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2TD, LineRenderPIZ2T, PointRenderPIZ2T,
    //		ZbRenderFaceGroup_FaceIV, TriangleRenderPIZ2TIA, LineRenderPIZ2TI, PointRenderPIZ2TI,
    ////		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V, CM_W | CM_U | CM_V,
    //	},
    //	{
    //		IDENT("Lit smooth texture mapped (linear, dithered) 256x256"),
    //		MT_MASK_TP | ZB_MATF_NO_SKIP | BR_MATF_LIGHT | BR_MATF_DITHER, BR_MATF_SMOOTH | BR_MATF_LIGHT |
    //ZB_MATF_HAS_MAP | ZB_MATF_NO_SKIP | BR_MATF_DITHER,
    //		BR_PMT_INDEX_8, 256, 256,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2TID, LineRenderPIZ2TI, PointRenderPIZ2TI,
    //		ZbRenderFaceGroup_FaceIV, TriangleRenderPIZ2TIA, LineRenderPIZ2TI, PointRenderPIZ2TI,
    ////		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V | CM_I, CM_W | CM_U | CM_V | CM_I,
    //	},
    //	{
    //		IDENT("Texture mapped (linear) 256x256"),
    //		MT_MASK_T | ZB_MATF_NO_SKIP | BR_MATF_LIGHT | BR_MATF_PRELIT, ZB_MATF_HAS_MAP | ZB_MATF_NO_SKIP,
    //		BR_PMT_INDEX_8, 256, 256,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2T, LineRenderPIZ2T, PointRenderPIZ2T,
    //		ZbRenderFaceGroup_FaceIV, TriangleRenderPIZ2TIA, LineRenderPIZ2TI, PointRenderPIZ2TI,
    ////		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V , CM_U | CM_V,
    //	},
    //	{
    //		IDENT("Lit flat texture mapped (linear) 256x256"),
    //		MT_MASK_T | ZB_MATF_NO_SKIP | BR_MATF_LIGHT, BR_MATF_LIGHT | ZB_MATF_HAS_MAP | ZB_MATF_NO_SKIP,
    //		BR_PMT_INDEX_8, 256, 256,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2I, LineRenderPIZ2I, PointRenderPIZ2,
    //		ZbRenderFaceGroup_FaceIV, TriangleRenderPIZ2TIA, LineRenderPIZ2TI, PointRenderPIZ2TI,
    ////		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V | CM_I, CM_U | CM_V | CM_I,
    //	},
    //	{
    //		IDENT("Lit smooth texture mapped (linear) 256x256"),
    //		MT_MASK_T | ZB_MATF_NO_SKIP | BR_MATF_LIGHT, BR_MATF_SMOOTH | BR_MATF_LIGHT | ZB_MATF_HAS_MAP |
    //ZB_MATF_NO_SKIP,
    //		BR_PMT_INDEX_8, 256, 256,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2I, LineRenderPIZ2I, PointRenderPIZ2,
    //		ZbRenderFaceGroup_FaceIV, TriangleRenderPIZ2TIA, LineRenderPIZ2TI, PointRenderPIZ2TI,
    ////		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V | CM_I, CM_U | CM_V | CM_I,
    //	},

    /*
     * Arbitary width general texture mapping
     */
    {
        IDENT("Texture mapped (linear) AW"),
        MT_MASK_T | BR_MATF_LIGHT | BR_MATF_PRELIT | ZB_MATF_MAP_TRANSPARENT,
        ZB_MATF_HAS_MAP | ZB_MATF_MAP_TRANSPARENT,
        BR_PMT_INDEX_8,
        0,
        0,
        //		ZbRenderFaceGroup, TriangleRenderPIZ2TA, LineRenderPIZ2T, PointRenderPIZ2T,
        ZbRenderFaceGroup_FaceIV,
        TriangleRenderPIZ2TIA,
        LineRenderPIZ2TI,
        PointRenderPIZ2TI,
        //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
        CM_COORDS | CM_U | CM_V,
        CM_U | CM_V,
    },
    {
        IDENT("Lit flat texture mapped (linear) AW"),
        MT_MASK_T | BR_MATF_LIGHT | ZB_MATF_MAP_TRANSPARENT,
        BR_MATF_LIGHT | ZB_MATF_HAS_MAP | ZB_MATF_MAP_TRANSPARENT,
        BR_PMT_INDEX_8,
        0,
        0,
        ZbRenderFaceGroup_FaceIV,
        TriangleRenderPIZ2TIA,
        LineRenderPIZ2TI,
        PointRenderPIZ2TI,
        //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
        CM_COORDS | CM_U | CM_V | CM_I,
        CM_U | CM_V | CM_I,
    },
    {
        IDENT("Lit smooth texture mapped (linear) AW"),
        MT_MASK_T | BR_MATF_LIGHT | ZB_MATF_MAP_TRANSPARENT,
        BR_MATF_SMOOTH | BR_MATF_LIGHT | ZB_MATF_HAS_MAP | ZB_MATF_MAP_TRANSPARENT,
        BR_PMT_INDEX_8,
        0,
        0,
        ZbRenderFaceGroup,
        TriangleRenderPIZ2TIA,
        LineRenderPIZ2TI,
        PointRenderPIZ2TI,
        CM_COORDS | CM_U | CM_V | CM_I,
        CM_U | CM_V | CM_I,
    },

    //	/*
    //	 * Arbitary width general texture mapping + decal
    //	 */
    //	{
    //		IDENT("Texture mapped decal (linear) AW"),
    //		MT_MASK_T | BR_MATF_LIGHT | BR_MATF_PRELIT, ZB_MATF_HAS_MAP | BR_MATF_DECAL,
    //		BR_PMT_INDEX_8, 0, 0,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2TAD, LineRenderPIZ2I, PointRenderPIZ2,
    //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V , CM_U | CM_V,
    //	},
    //	{
    //		IDENT("Lit flat texture mapped decal (linear) AW"),
    //		MT_MASK_T | BR_MATF_LIGHT, BR_MATF_LIGHT | ZB_MATF_HAS_MAP  | BR_MATF_DECAL,
    //		BR_PMT_INDEX_8, 0, 0,
    ////		ZbRenderFaceGroup_FaceIV, TriangleRenderPIZ2TIAD, LineRenderPIZ2I, PointRenderPIZ2,
    //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V | CM_I, CM_U | CM_V | CM_I,
    //	},
    //	{
    //		IDENT("Lit smooth texture mapped decal (linear) AW"),
    //		MT_MASK_T | BR_MATF_LIGHT, BR_MATF_SMOOTH | BR_MATF_LIGHT | ZB_MATF_HAS_MAP | BR_MATF_DECAL,
    //		BR_PMT_INDEX_8, 0, 0,
    ////		ZbRenderFaceGroup, TriangleRenderPIZ2TIAD, LineRenderPIZ2I, PointRenderPIZ2,
    //		ZbRenderFaceGroup, TriangleRender_Null, LineRender_Null, PointRender_Null,
    //		CM_COORDS | CM_U | CM_V | CM_I, CM_U | CM_V | CM_I,
    //	},
    //
    {
        IDENT("Smooth shading"),
        BR_MATF_SMOOTH,
        BR_MATF_SMOOTH,
        0,
        0,
        0,
        //		ZbRenderFaceGroup_FaceIV, TriangleRenderPIZ2TIA, LineRenderPIZ2TI, PointRenderPIZ2TI,
        ZbRenderFaceGroup,
        TriangleRenderPIZ2I,
        LineRenderPIZ2I,
        PointRenderPIZ2,
        CM_COORDS | CM_I,
        CM_I,
    },
    {
        IDENT("Flat shading"),
        0,
        0,
        0,
        0,
        0,
        //		ZbRenderFaceGroup_FaceI, TriangleRenderPIZ2, LineRenderPIZ2I, PointRenderPIZ2,
        //		ZbRenderFaceGroup_FaceIV, TriangleRenderPIZ2TIA, LineRenderPIZ2TI, PointRenderPIZ2TI,
        ZbRenderFaceGroup,
        TriangleRender_Null,
        LineRender_Null,
        PointRender_Null,
        CM_COORDS,
        0,
    },
};

/*
 * Material types for RGB_555, DEPTH_16
 */
static struct zb_material_type mat_types_rgb_555[] = {
    {
        IDENT("Texture mapped (linear) AW"),
        MT_MASK_T | BR_MATF_LIGHT | BR_MATF_PRELIT,
        ZB_MATF_HAS_MAP,
        BR_PMT_RGB_555,
        0,
        0,
        ZbRenderFaceGroup_FaceIV,
        TriangleRenderPIZ2TIA_RGB_555,
        LineRenderPIZ2T_RGB_555,
        PointRenderPIZ2T_RGB_555,
        CM_COORDS | CM_U | CM_V,
        CM_U | CM_V,
    },
    {
        IDENT("Lit flat texture mapped (linear) AW"),
        MT_MASK_T | BR_MATF_LIGHT | ZB_MATF_MAP_TRANSPARENT,
        BR_MATF_LIGHT | ZB_MATF_HAS_MAP | ZB_MATF_MAP_TRANSPARENT,
        BR_PMT_RGB_555,
        0,
        0,
        ZbRenderFaceGroup_FaceIV,
        TriangleRenderPIZ2TIA_RGB_555,
        LineRenderPIZ2T_RGB_555,
        PointRenderPIZ2T_RGB_555,
        CM_COORDS | CM_U | CM_V | CM_I,
        CM_U | CM_V | CM_I,
    },
    {
        IDENT("Lit smooth texture mapped (linear) AW"),
        MT_MASK_T | BR_MATF_LIGHT | ZB_MATF_MAP_TRANSPARENT,
        BR_MATF_SMOOTH | BR_MATF_LIGHT | ZB_MATF_HAS_MAP | ZB_MATF_MAP_TRANSPARENT,
        BR_PMT_RGB_555,
        0,
        0,
        ZbRenderFaceGroup,
        TriangleRenderPIZ2TIA_RGB_555,
        LineRenderPIZ2T_RGB_555,
        PointRenderPIZ2T_RGB_555,
        CM_COORDS | CM_U | CM_V | CM_I,
        CM_U | CM_V | CM_I,
    },

    {
        IDENT("Smooth shading"),
        MT_MASK,
        BR_MATF_SMOOTH,
        0,
        0,
        0,
        ZbRenderFaceGroup,
        TriangleRenderPIZ2I_RGB_555,
        LineRenderPIZ2I_RGB_555,
        PointRenderPIZ2_RGB_555,
        CM_COORDS | CM_R | CM_G | CM_B,
        CM_R | CM_G | CM_B,
    },
    {
        IDENT("Flat shading"),
        0,
        0,
        0,
        0,
        0,
        ZbRenderFaceGroup_FaceRGB,
        TriangleRenderPIZ2_RGB_555,
        LineRenderPIZ2I_RGB_555,
        PointRenderPIZ2_RGB_555,
        CM_COORDS,
        CM_R | CM_G | CM_B,
    },
};

/*
 * Material types for RGB_888, DEPTH_16
 */
static struct zb_material_type mat_types_rgb_888[] = {
    {
        IDENT("Texture mapped (linear) AW"),
        MT_MASK_T | BR_MATF_LIGHT | BR_MATF_PRELIT,
        ZB_MATF_HAS_MAP,
        BR_PMT_RGB_888,
        0,
        0,
        ZbRenderFaceGroup_FaceIV,
        TriangleRenderPIZ2TIA_RGB_888,
        LineRenderPIZ2T_RGB_888,
        PointRenderPIZ2T_RGB_888,
        CM_COORDS | CM_U | CM_V,
        CM_U | CM_V,
    },
    {
        IDENT("Lit flat texture mapped (linear) AW"),
        MT_MASK_T | BR_MATF_LIGHT | ZB_MATF_MAP_TRANSPARENT,
        BR_MATF_LIGHT | ZB_MATF_HAS_MAP | ZB_MATF_MAP_TRANSPARENT,
        BR_PMT_RGB_888,
        0,
        0,
        ZbRenderFaceGroup_FaceIV,
        TriangleRenderPIZ2TIA_RGB_888,
        LineRenderPIZ2T_RGB_888,
        PointRenderPIZ2T_RGB_888,
        CM_COORDS | CM_U | CM_V | CM_I,
        CM_U | CM_V | CM_I,
    },
    {
        IDENT("Lit smooth texture mapped (linear) AW"),
        MT_MASK_T | BR_MATF_LIGHT | ZB_MATF_MAP_TRANSPARENT,
        BR_MATF_SMOOTH | BR_MATF_LIGHT | ZB_MATF_HAS_MAP | ZB_MATF_MAP_TRANSPARENT,
        BR_PMT_RGB_888,
        0,
        0,
        ZbRenderFaceGroup,
        TriangleRenderPIZ2TIA_RGB_888,
        LineRenderPIZ2T_RGB_888,
        PointRenderPIZ2T_RGB_888,
        CM_COORDS | CM_U | CM_V | CM_I,
        CM_U | CM_V | CM_I,
    },

    {
        IDENT("Smooth shading"),
        BR_MATF_SMOOTH,
        BR_MATF_SMOOTH,
        0,
        0,
        0,
        ZbRenderFaceGroup,
        TriangleRenderPIZ2I_RGB_888,
        LineRenderPIZ2I_RGB_888,
        PointRenderPIZ2_RGB_888,
        CM_COORDS | CM_R | CM_G | CM_B,
        CM_R | CM_G | CM_B,
    },
    {
        IDENT("Flat shading"),
        0,
        0,
        0,
        0,
        0,
        ZbRenderFaceGroup_FaceRGB,
        TriangleRenderPIZ2_RGB_888,
        LineRenderPIZ2I_RGB_888,
        PointRenderPIZ2_RGB_888,
        CM_COORDS,
        CM_R | CM_G | CM_B,
    },
};

/*
 * Table of valid render types
 */
static struct zb_render_type rtype_table[] = {
    {
        IDENT("8 bit indexed, 16 bit Z"),
        BR_PMT_INDEX_8,
        BR_PMT_DEPTH_16,
        0,
        1,
        2,
        0,
        mat_types_index_8,
        BR_ASIZE(mat_types_index_8),
    },
    {
        IDENT("15 bit true colour, 16 bit Z"),
        BR_PMT_RGB_555,
        BR_PMT_DEPTH_16,
        0,
        2,
        2,
        1,
        mat_types_rgb_555,
        BR_ASIZE(mat_types_rgb_555),
    },
    {
        IDENT("24 true colour, 16 bit Z"),
        BR_PMT_RGB_888,
        BR_PMT_DEPTH_16,
        0,
        3,
        2,
        1,
        mat_types_rgb_888,
        BR_ASIZE(mat_types_rgb_888),
    },
};

void BR_PUBLIC_ENTRY BrZbBegin(br_uint_8 colour_type, br_uint_8 depth_type)
{
    int i;

    /*
     * See if we can support pixelmap types provided
     */
    for (i = 0; i < BR_ASIZE(rtype_table); i++)
        if (colour_type == rtype_table[i].colour_type && depth_type == rtype_table[i].depth_type)
            break;

    if (i >= BR_ASIZE(rtype_table))
        BR_ERROR("BrZbBegin: invalid types for rendering");

    /*
     * Remember the renderer type structure
     */
    zb.type = rtype_table + i;

    /*
     * Setup anchor block for resources associated with ZB renderer
     */
    zb.res = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);

    zb.row_width = 0;

    /*
     * Hook update functions
     */
    fw.material_update = ZbMaterialUpdate;

    /*
     * Update the default material and model
     */
    BrMaterialUpdate(fw.default_material, BR_MATU_ALL);
    BrModelUpdate(fw.default_model, BR_MODU_ALL);
}

void BR_PUBLIC_ENTRY BrZbEnd(void)
{
    if (zb.res)
        BrResFree(zb.res);

    /*
     * Clear out zb structure
     */
    memset(&zb, 0, sizeof(zb));
}

//#if 0		// Hawkeye
/*
 * Dummy triangle renderer for unimplemented ops.
 */
void BR_ASM_CALL TriangleRender_Null(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                     struct temp_vertex_fixed *v2)
{
}

/*
 * Dummy line renderer for unimplemented ops.
 */
void BR_ASM_CALL LineRender_Null(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1)
{
}

/*
 * Dummy point renderer for unimplemented ops.
 */
void BR_ASM_CALL PointRender_Null(struct temp_vertex_fixed *v0)
{
}
//#endif



