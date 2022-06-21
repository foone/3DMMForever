/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: zbiproto.h 1.24 1995/08/31 16:47:52 sam Exp $
 * $Locker:  $
 *
 * Prototypes for functions internal to z-buffer renderer
 */
#ifndef _ZBIPROTO_H_
#define _ZBIPROTO_H_
#ifndef NO_PROTOTYPES

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * ti8_piz.asm
     */
    /*
     * 8 bit indexed, perfect, integer, Z buffered (16 bit)
     */
    void BR_ASM_CALL TriangleRenderPIZ2(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                        struct temp_vertex_fixed *v2);

    void BR_ASM_CALL TriangleRenderPIZ2I(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                         struct temp_vertex_fixed *v2);

    void BR_ASM_CALL TriangleRenderPIZ2T(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                         struct temp_vertex_fixed *v2);

    void BR_ASM_CALL TriangleRenderPIZ2TI(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                          struct temp_vertex_fixed *v2);

    void BR_ASM_CALL TriangleRenderPIZ2TD(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                          struct temp_vertex_fixed *v2);

    void BR_ASM_CALL TriangleRenderPIZ2TID(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                           struct temp_vertex_fixed *v2);

    /*
     * tt24_piz.asm
     */
    /*
     * 24 bit true colour, perfect, integer, Z buffered (16 bit)
     */
    void BR_ASM_CALL TriangleRenderPIZ2_RGB_888(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                                struct temp_vertex_fixed *v2);

    void BR_ASM_CALL TriangleRenderPIZ2I_RGB_888(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                                 struct temp_vertex_fixed *v2);

    /*
     * tt15_piz.asm
     */
    /*
     * 15 bit true colour, perfect, integer, Z buffered (16 bit)
     */
    void BR_ASM_CALL TriangleRenderPIZ2_RGB_555(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                                struct temp_vertex_fixed *v2);

    void BR_ASM_CALL TriangleRenderPIZ2I_RGB_555(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                                 struct temp_vertex_fixed *v2);

    /*
     * perspz.c
     */
    void BR_ASM_CALL TriangleRenderPIZ2TIP1024(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                               struct temp_vertex_fixed *c);
    void BR_ASM_CALL TriangleRenderPIZ2TIP256(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                              struct temp_vertex_fixed *c);
    void BR_ASM_CALL TriangleRenderPIZ2TIP64(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                             struct temp_vertex_fixed *c);
    void BR_ASM_CALL TriangleRenderPIZ2TP1024(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                              struct temp_vertex_fixed *c);
    void BR_ASM_CALL TriangleRenderPIZ2TP256(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                             struct temp_vertex_fixed *c);
    void BR_ASM_CALL TriangleRenderPIZ2TP64(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                            struct temp_vertex_fixed *c);
    void BR_ASM_CALL TriangleRenderPIZ2TPD1024(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                               struct temp_vertex_fixed *c);

    /*
     * awtmz.c
     */
    void BR_ASM_CALL TriangleRenderPIZ2TA(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                          struct temp_vertex_fixed *c);
    void BR_ASM_CALL TriangleRenderPIZ2TA_RGB_555(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                                  struct temp_vertex_fixed *c);
    void BR_ASM_CALL TriangleRenderPIZ2TA_RGB_888(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                                  struct temp_vertex_fixed *c);

    void BR_ASM_CALL TriangleRenderPIZ2TIA(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                           struct temp_vertex_fixed *c);
    void BR_ASM_CALL TriangleRenderPIZ2TIA_RGB_555(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                                   struct temp_vertex_fixed *c);
    void BR_ASM_CALL TriangleRenderPIZ2TIA_RGB_888(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                                   struct temp_vertex_fixed *c);

    /*
     * mesh386.asm
     */
    br_int_32 BR_ASM_CALL ZbOSFFVGroupCulled_A(br_face *fp, struct temp_face *tfp, int count);
    br_int_32 BR_ASM_CALL ZbOSFFVGroupCulledLit_A(br_face *fp, struct temp_face *tfp, int count);

    br_uint_32 BR_ASM_CALL ZbOSTVGroup_A(br_vertex *vp, struct temp_vertex_fixed *tvp, int count, br_uint_8 *countp);
    br_uint_32 BR_ASM_CALL ZbOSTVGroupLit_A(br_vertex *vp, struct temp_vertex_fixed *tvp, int count, br_uint_8 *countp);

    br_uint_32 BR_ASM_CALL ZbOSTVGroupBC_A(br_vertex *vp, struct temp_vertex_fixed *tvp, int count, br_uint_8 *countp);
    br_uint_32 BR_ASM_CALL ZbOSTVGroupLitBC_A(br_vertex *vp, struct temp_vertex_fixed *tvp, int count,
                                              br_uint_8 *countp);

    br_uint_32 BR_ASM_CALL ZbOSCopyModelToScreen_A(void);

    /*
     * zbmesh.c
     */
    void ZbMeshRender(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen);

    void ZbFindVisibleFaces(void);
    void ZbFindVisibleFacesPar(void);
    void ZbTransformVertices(void);
    void ZbFindFacesAndVertices(void);
    void ZbFindVertexParameters(void);

    void BR_ASM_CALL ZbRenderFaceGroup(br_face_group *gp, struct temp_face *tfp);
    void BR_ASM_CALL ZbRenderFaceGroup_FaceI(br_face_group *gp, struct temp_face *tfp);
    void BR_ASM_CALL ZbRenderFaceGroup_FaceIV(br_face_group *gp, struct temp_face *tfp);
    void BR_ASM_CALL ZbRenderFaceGroup_FaceRGB(br_face_group *gp, struct temp_face *tfp);

#if !BASED_FIXED
    void ZbConvertComponents(br_fixed_ls *dest, br_scalar *src, br_uint_32 mask);
#endif

    /*
     * zbmeshp.c
     */
    void ZbPointFindVertexParameters(void);
    void ZbMeshRenderPoints(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen);

    void BR_ASM_CALL PointRenderPIZ2(struct temp_vertex_fixed *v0);
    void BR_ASM_CALL PointRenderPIZ2T(struct temp_vertex_fixed *v0);
    void BR_ASM_CALL PointRenderPIZ2TI(struct temp_vertex_fixed *v0);

    void BR_ASM_CALL PointRenderPIZ2_RGB_888(struct temp_vertex_fixed *v0);
    void BR_ASM_CALL PointRenderPIZ2T_RGB_888(struct temp_vertex_fixed *v0);

    void BR_ASM_CALL PointRenderPIZ2_RGB_555(struct temp_vertex_fixed *v0);
    void BR_ASM_CALL PointRenderPIZ2T_RGB_555(struct temp_vertex_fixed *v0);

    /*
     * zbmeshe.c
     */
    void ZbMeshRenderEdges(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen);

    void BR_ASM_CALL LineRenderPIZ2I(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);
    void BR_ASM_CALL LineRenderPIZ2T(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);
    void BR_ASM_CALL LineRenderPIZ2TI(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);

    void BR_ASM_CALL LineRenderPFZ2I(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);
    void BR_ASM_CALL LineRenderPFZ2I555(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);
    void BR_ASM_CALL LineRenderPFZ2I888(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);
    void BR_ASM_CALL LineRenderPFZ4I(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);

    void BR_ASM_CALL LineRenderPIZ2T_RGB_888(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);
    void BR_ASM_CALL LineRenderPIZ2I_RGB_888(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);
    void BR_ASM_CALL LineRenderPIZ2T_RGB_555(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);
    void BR_ASM_CALL LineRenderPIZ2I_RGB_555(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1);

    /*
     * zbrendr.c
     */
    void ZbNullRender(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen);

    /*
     * bbox.c
     */
    void ZbBoundingBoxRenderPoints(br_actor *actor, br_model *model, br_material *material, br_uint_8 style,
                                   int on_screen);

    void ZbBoundingBoxRenderEdges(br_actor *actor, br_model *model, br_material *material, br_uint_8 style,
                                  int on_screen);

    void ZbBoundingBoxRenderFaces(br_actor *actor, br_model *model, br_material *material, br_uint_8 style,
                                  int on_screen);

    void StartBoundingBox(int on_screen);
    void EndBoundingBox(int on_screen);

    /*
     * zbclip.c
     */
    struct clip_vertex *ZbFaceClip(br_face *fp, struct temp_face *tfp, int mask, int *n_out);
    struct clip_vertex *ZbTempClip(struct temp_vertex *tvp, struct temp_face *tfp, int mask, int *n_out);

    /*
     * zbmatl.c
     */
    void ZbMaterialUpdate(br_material *mat, br_uint_16 flags);

    /*
     * decalz.c
     */
    void BR_ASM_CALL TriangleRenderPIZ2TAD(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                           struct temp_vertex_fixed *c);
    void BR_ASM_CALL TriangleRenderPIZ2TIAD(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                            struct temp_vertex_fixed *c);

#ifdef __cplusplus
};
#endif
#endif
#endif

