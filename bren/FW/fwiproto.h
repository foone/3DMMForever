/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: fwiproto.h 1.29 1995/08/31 16:29:28 sam Exp $
 * $Locker:  $
 *
 * Prototypes for functions internal to framework
 */
#ifndef _FWIPROTO_H_
#define _FWIPROTO_H_
#ifndef NO_PROTOTYPES

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * surface.c
     */
    void SurfacePerScene(br_actor *world, int true_colour);
    void SurfacePerModel(void);
    void SurfacePerMaterial(br_material *mat);
    void BR_SURFACE_CALL CopyComponents(br_vertex *v, br_fvector3 *normal, br_scalar *comp);

    /*
     * light8.c
     */
    br_uint_32 BR_SURFACE_CALL LightingFaceIndex(br_vertex *v, br_face *fp, int reversed);
    void BR_SURFACE_CALL LightingIndex(br_vertex *v, br_fvector3 *n, br_scalar *comp);
    void BR_SURFACE_CALL LightingIndexCopyVertex(br_vertex *v, br_fvector3 *n, br_scalar *comp);
    void BR_SURFACE_CALL LightingIndexCopyMaterial(br_vertex *v, br_fvector3 *n, br_scalar *comp);

    void LightingIndex_Dirn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp);
    void LightingIndex_Point(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp);
    void LightingIndex_PointAttn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp);
    void LightingIndex_Spot(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp);
    void LightingIndex_SpotAttn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp);

    void BR_SURFACE_CALL LightingIndex_1MD(br_vertex *v, br_fvector3 *n, br_scalar *comp);
    void BR_SURFACE_CALL LightingIndex_1MDT(br_vertex *v, br_fvector3 *n, br_scalar *comp);

    /*
     * light24.c
     */
    br_uint_32 BR_SURFACE_CALL LightingFaceColour(br_vertex *v, br_face *fp, int reversed);
    void BR_SURFACE_CALL LightingColour(br_vertex *v, br_fvector3 *n, br_scalar *comp);
    void BR_SURFACE_CALL LightingColourCopyVertex(br_vertex *v, br_fvector3 *n, br_scalar *comp);
    void BR_SURFACE_CALL LightingColourCopyMaterial(br_vertex *v, br_fvector3 *n, br_scalar *comp);

    void LightingColour_Dirn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp);
    void LightingColour_Point(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp);
    void LightingColour_PointAttn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp);
    void LightingColour_Spot(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp);
    void LightingColour_SpotAttn(br_vector3 *p, br_fvector3 *n, br_active_light *alp, br_scalar *lcomp);

    void BR_SURFACE_CALL LightingColour_1MD(br_vertex *v, br_fvector3 *n, br_scalar *comp);
    void BR_SURFACE_CALL LightingColour_1MDT(br_vertex *v, br_fvector3 *n, br_scalar *comp);

    /*
     * envmap.c
     */
    void BR_SURFACE_CALL MapEnvironmentInfinite2D(br_vertex *v, br_fvector3 *normal, br_scalar *comp);
    void BR_SURFACE_CALL MapEnvironmentLocal2D(br_vertex *v, br_fvector3 *normal, br_scalar *comp);
    void BR_SURFACE_CALL MapFromVertex(br_vertex *v, br_fvector3 *normal, br_scalar *comp);
    void BR_SURFACE_CALL MapFromVertexOnly(br_vertex *v, br_fvector3 *normal, br_scalar *comp);

    /*
     * rendsupt.c
     */
    extern br_material default_material;

    /*
     * actsupt.c
     */
    int BrActorToRoot(br_actor *a, br_actor *root, br_matrix34 *m);
    void BrVector3EyeInModel(br_vector3 *eye_m);
    int BrCameraToScreenMatrix4(br_matrix4 *mat, br_actor *camera);

    /*
     * vector.c - private vector functions
     */
    br_scalar BrFVector2Dot(br_fvector2 *v1, br_vector2 *v2);
    void BrVector3CopyF(br_vector3 *v1, br_fvector3 *v2);
    void BrVector3ScaleF(br_vector3 *v1, br_fvector3 *v2, br_scalar s);
    br_scalar BrFVector3Dot(br_fvector3 *v1, br_vector3 *v2);
    void BrFVector3Normalise(br_fvector3 *v1, br_vector3 *v2);
    void BrFVector3NormaliseQuick(br_fvector3 *v1, br_vector3 *v2);
    void BrFVector3NormaliseLP(br_fvector3 *v1, br_vector3 *v2);

    /*
     * matrix.c - private matrix functions
     */
    void BrMatrix34TApplyFV(br_vector3 *A, br_fvector3 *B, br_matrix34 *C);

    /*
     * register.c
     */
    int NamePatternMatch(char *p, char *s);
    void *RegistryNew(br_registry *reg);
    void *RegistryClear(br_registry *reg);
    void *RegistryAdd(br_registry *reg, void *item);
    int RegistryAddMany(br_registry *reg, void **item, int n);
    void *RegistryRemove(br_registry *reg, void *item);
    int RegistryRemoveMany(br_registry *reg, void **item, int n);
    void *RegistryFind(br_registry *reg, char *pattern);
    int RegistryFindMany(br_registry *reg, char *pattern, void **item, int n);
    int RegistryCount(br_registry *reg, char *pattern);
    int RegistryEnum(br_registry *reg, char *pattern, br_enum_cbfn *callback, void *arg);

    /*
     * scrstr.c
     */
    extern char _br_scratch_string[];

    /*
     * pool.c
     */
    void BR_ASM_CALLBACK BrPoolAddChunk(br_pool *pool);

    /*
     * fixed386.asm
     */

#if 1 /* DEBUG */
    /*
     * Debugging printf
     */
    int BrLogPrintf(char *fmt, ...);
#endif

    /*
     * fixed.c
     */
    extern unsigned long BR_ASM_DATA _reciprocal[2048];

    /*
     * transform.c
     */
    extern br_uint_8 _CombineTransforms[BR_TRANSFORM_MAX][BR_TRANSFORM_MAX];

#define COMBINE_TRANSFORMS(a, b) (_CombineTransforms[(a)][(b)])
#define IS_LP(a) ((a) != BR_TRANSFORM_MATRIX34)

    /*
     * pmmemops.c
     */
    extern br_context _BrMemoryContext;
    extern br_device _BrMemoryDevice;

    void BR_ASM_CALL _BrPmMemFill(br_context *ctx, br_pixelmap *dst, br_uint_32 colour);
    void BR_ASM_CALL _BrPmMemRectangleCopyTo(br_context *ctx, br_pixelmap *dst, br_uint_16 dx, br_uint_16 dy,
                                             br_pixelmap *src, br_uint_16 sx, br_uint_16 sy, br_uint_16 w,
                                             br_uint_16 h);
    void BR_ASM_CALL _BrPmMemRectangleCopyFrom(br_context *ctx, br_pixelmap *dst, br_uint_16 dx, br_uint_16 dy,
                                               br_pixelmap *src, br_uint_16 sx, br_uint_16 sy, br_uint_16 w,
                                               br_uint_16 h);
    void BR_ASM_CALL _BrPmMemRectangleFill(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y, br_uint_16 w,
                                           br_uint_16 h, br_uint_32 colour);
    void BR_ASM_CALL _BrPmMemDirtyRectangleCopy(br_context *ctx, br_pixelmap *dst, br_pixelmap *src, br_uint_16 x,
                                                br_uint_16 y, br_uint_16 w, br_uint_16 h);
    void BR_ASM_CALL _BrPmMemDirtyRectangleFill(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y,
                                                br_uint_16 w, br_uint_16 h, br_uint_32 colour);
    void BR_ASM_CALL _BrPmMemPixelSet(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y, br_uint_32 colour);
    br_uint_32 BR_ASM_CALL _BrPmMemPixelGet(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y);
    void BR_ASM_CALL _BrPmMemCopy(br_context *ctx, br_pixelmap *dst, br_pixelmap *src);
    void BR_ASM_CALL _BrPmMemLine(br_context *ctx, br_pixelmap *dst, br_int_16 x1, br_int_16 y1, br_int_16 x2,
                                  br_int_16 y2, br_uint_32 colour);
    void BR_ASM_CALL _BrPmMemCopyBits(br_context *ctx, br_pixelmap *dst, br_int_16 x, br_int_16 y, br_uint_8 *src,
                                      br_uint_16 s_stride, br_uint_16 start_bit, br_uint_16 end_bit, br_uint_16 nrows,
                                      br_uint_32 colour);
    void BR_ASM_CALL _BrPmMemDoubleBuffer(br_context *ctx, br_pixelmap *dest, br_pixelmap *src);
    br_pixelmap *BR_ASM_CALL _BrPmMemMatch(br_context *ctx, br_pixelmap *src, br_uint_8 match_type);
    br_pixelmap *BR_ASM_CALL _BrPmMemClone(br_context *ctx, br_pixelmap *dest, br_pixelmap *src);
    void BR_ASM_CALL _BrPmMemFree(br_context *ctx, br_pixelmap *src);

    /*
     * pmgenops.c
     */
    void BR_ASM_CALL _BrGenericLine(br_context *ctx, br_pixelmap *dst, br_int_16 x1, br_int_16 y1, br_int_16 x2,
                                    br_int_16 y2, br_uint_32 colour);
    void BR_ASM_CALL _BrGenericDoubleBuffer(br_context *ctx, br_pixelmap *dest, br_pixelmap *src);
    br_pixelmap *BR_ASM_CALL _BrGenericMatch(br_context *ctx, br_pixelmap *src, br_uint_8 match_type);
    br_pixelmap *BR_ASM_CALL _BrGenericClone(br_context *ctx, br_pixelmap *src);
    void BR_ASM_CALL _BrGenericFree(br_context *ctx, br_pixelmap *src);
    void BR_ASM_CALL _BrGenericDirtyRectangleCopy(br_context *ctx, br_pixelmap *dst, br_pixelmap *src, br_uint_16 x,
                                                  br_uint_16 y, br_uint_16 w, br_uint_16 h);
    void BR_ASM_CALL _BrGenericDirtyRectangleFill(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y,
                                                  br_uint_16 w, br_uint_16 h, br_uint_32 colour);

    void BR_ASM_CALL _BrGenericRectangle(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y, br_uint_16 w,
                                         br_uint_16 h, br_uint_32 colour);
    void BR_ASM_CALL _BrGenericRectangle2(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y, br_uint_16 w,
                                          br_uint_16 h, br_uint_32 colour1, br_uint_32 colour2);

    void BR_ASM_CALL _BrGenericRectangleCopy(br_context *ctx, br_pixelmap *dst, br_uint_16 dx, br_uint_16 dy,
                                             br_pixelmap *src, br_uint_16 sx, br_uint_16 sy, br_uint_16 w,
                                             br_uint_16 h);

    void BR_ASM_CALL _BrGenericCopy(br_context *ctx, br_pixelmap *dst, br_pixelmap *src);

    /*
     * fonts
     */
    extern struct br_font BR_ASM_DATA _FontFixed3x5;
    extern struct br_font BR_ASM_DATA _FontProp4x6;
    extern struct br_font BR_ASM_DATA _FontProp7x9;

    /*
     * memloops.asm
     */
    void BR_ASM_CALL _MemCopyBits_A(char *dest, br_int_32 d_stride, br_uint_8 *src, br_uint_32 s_stride,
                                    br_uint_32 start_bit, br_uint_32 end_bit, br_uint_32 nrows, br_uint_32 bpp,
                                    br_uint_32 colour);

    void BR_ASM_CALL _MemFill_A(char *dest, br_uint_32 pixels, br_uint_32 bpp, br_uint_32 colour);
    void BR_ASM_CALL _MemRectFill_A(char *dest, br_uint_32 pwidth, br_uint_32 pheight, br_int_32 d_stride,
                                    br_uint_32 bpp, br_uint_32 colour);

    void BR_ASM_CALL _MemRectCopy_A(char *dest, char *src, br_uint_32 pwidth, br_uint_32 pheight, br_int_32 d_stride,
                                    br_int_32 s_stride, br_uint_32 bpp);

    void BR_ASM_CALL _MemCopy_A(char *dest, char *src, br_uint_32 pixels, br_uint_32 bpp);
    void BR_ASM_CALL _MemPixelSet(char *dest, br_uint_32 bytes, br_uint_32 colour);

    br_uint_32 BR_ASM_CALL _MemPixelGet(char *dest, br_uint_32 bytes);

    /*
     * resource.c
     */
    br_uint_32 BR_PUBLIC_ENTRY BrResCheck(void *vres, int no_tag);
    void BR_PUBLIC_ENTRY BrResDump(void *vres, void (*putline)(char *str, void *arg), void *arg);

    /*
     * pixelmap.c
     */
    br_uint_16 BR_PUBLIC_ENTRY BrPixelmapFileSize(br_pixelmap *pm);

    /*
     * brexcept.c
     */
    struct br_exception_handler *_BrCatch(void);
    void _BrThrow(br_int_32 value);
    void *_BrExceptionResource(void);
    extern br_exception_handler *_BrCurrentHandler;

    /*
     * loader.c
     */
    br_image *BR_PUBLIC_ENTRY BrImageReference(char *name);
    void BR_PUBLIC_ENTRY BrImageDereference(br_image *image);
    void *BR_PUBLIC_ENTRY BrImageLookupName(br_image *img, char *name, br_uint_32 hint);
    void *BR_PUBLIC_ENTRY BrImageLookupOrdinal(br_image *img, br_uint_32 ordinal);

    void BR_CALLBACK _BrImageFree(void *res, br_uint_8 res_class, br_size_t size);

    /*
     * file.c
     */
    void BR_CALLBACK _BrFileFree(void *res, br_uint_8 res_class, br_size_t size);

    /*
     * error.c
     */
    void BrSetLastError(br_error type, void *value);

#ifdef __cplusplus
};
#endif
#endif
#endif
