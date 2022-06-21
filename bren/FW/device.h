/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: device.h 1.5 1995/07/28 19:01:38 sam Exp $
 * $Locker:  $
 *
 * Device structure - a function dispatch table for pixelmap operations
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * function list for pixelmaps->context
     */
    struct br_context;
    struct br_device;

    typedef void BR_ASM_CALL ctx_fill_cbfn(struct br_context *dev, br_pixelmap *dst, br_uint_32 colour);

    typedef void BR_ASM_CALL ctx_rectangle_cbfn(struct br_context *dev, br_pixelmap *dst, br_uint_16 x, br_uint_16 y,
                                                br_uint_16 w, br_uint_16 h, br_uint_32 colour);

    typedef void BR_ASM_CALL ctx_rectangle2_cbfn(struct br_context *dev, br_pixelmap *dst, br_uint_16 x, br_uint_16 y,
                                                 br_uint_16 w, br_uint_16 h, br_uint_32 colour1, br_uint_32 colour2);

    typedef void BR_ASM_CALL ctx_rectangle_copy_cbfn(struct br_context *dev, br_pixelmap *dst, br_uint_16 dx,
                                                     br_uint_16 dy, br_pixelmap *src, br_uint_16 sx, br_uint_16 sy,
                                                     br_uint_16 w, br_uint_16 h);

    typedef void BR_ASM_CALL ctx_rectangle_fill_cbfn(struct br_context *dev, br_pixelmap *dst, br_uint_16 x,
                                                     br_uint_16 y, br_uint_16 w, br_uint_16 h, br_uint_32 colour);

    typedef void BR_ASM_CALL ctx_dirty_rectangle_copy_cbfn(struct br_context *dev, br_pixelmap *dst, br_pixelmap *src,
                                                           br_uint_16 x, br_uint_16 y, br_uint_16 w, br_uint_16 h);

    typedef void BR_ASM_CALL ctx_dirty_rectangle_fill_cbfn(struct br_context *dev, br_pixelmap *dst, br_uint_16 x,
                                                           br_uint_16 y, br_uint_16 w, br_uint_16 h, br_uint_32 colour);

    typedef void BR_ASM_CALL ctx_pixel_set_cbfn(struct br_context *dev, br_pixelmap *dst, br_uint_16 x, br_uint_16 y,
                                                br_uint_32 colour);

    typedef br_uint_32 BR_ASM_CALL ctx_pixel_get_cbfn(struct br_context *dev, br_pixelmap *dst, br_uint_16 x,
                                                      br_uint_16 y);

    typedef void BR_ASM_CALL ctx_copy_cbfn(struct br_context *dev, br_pixelmap *dst, br_pixelmap *src);
    typedef void BR_ASM_CALL ctx_line_cbfn(struct br_context *dev, br_pixelmap *dst, br_int_16 x1, br_int_16 y1,
                                           br_int_16 x2, br_int_16 y2, br_uint_32 colour);
    typedef void BR_ASM_CALL ctx_copy_bits_cbfn(struct br_context *dev, br_pixelmap *dst, br_int_16 x, br_int_16 y,
                                                br_uint_8 *src, br_uint_16 s_stride, br_uint_16 start_bit,
                                                br_uint_16 end_bit, br_uint_16 nrows, br_uint_32 colour);

    typedef void BR_ASM_CALL ctx_double_buffer_cbfn(struct br_context *dev, br_pixelmap *dest, br_pixelmap *src);

    typedef br_pixelmap *BR_ASM_CALL ctx_match_cbfn(struct br_context *dev, br_pixelmap *src, br_uint_8 match_type);
    typedef br_pixelmap *BR_ASM_CALL ctx_clone_cbfn(struct br_context *dev, br_pixelmap *src);
    typedef void BR_ASM_CALL ctx_free_cbfn(struct br_context *dev, br_pixelmap *src);

    /*
     * Device context structure
     */
    typedef struct br_context
    {
        /*
         * General ID for this structure
         */
        char *identifier;

        /*
         * Pointer to underlying device
         */
        struct br_device *device;

        /*
         * Font for context
         */
        struct br_font *font;

        /**
         ** Function pointers for all the context operations
         **/

        /*
         * Pixelmap management operations
         */
        ctx_free_cbfn *free;
        ctx_match_cbfn *match;
        ctx_clone_cbfn *clone;
        ctx_double_buffer_cbfn *double_buffer;

        /*
         * Pixelmap copying
         */
        ctx_copy_cbfn *copy;
        ctx_copy_cbfn *copy_to;
        ctx_copy_cbfn *copy_from;

        /*
         * Rendering operations
         */
        ctx_fill_cbfn *fill;
        ctx_rectangle_cbfn *rectangle;
        ctx_rectangle2_cbfn *rectangle2;
        ctx_rectangle_copy_cbfn *rectangle_copy;
        ctx_rectangle_copy_cbfn *rectangle_copy_to;
        ctx_rectangle_copy_cbfn *rectangle_copy_from;
        ctx_rectangle_fill_cbfn *rectangle_fill;
        ctx_dirty_rectangle_copy_cbfn *dirty_rectangle_copy;
        ctx_dirty_rectangle_fill_cbfn *dirty_rectangle_fill;
        ctx_pixel_set_cbfn *pixel_set;
        ctx_pixel_get_cbfn *pixel_get;
        ctx_line_cbfn *line;
        ctx_copy_bits_cbfn *copy_bits;

    } br_context;

    /*
     * Generic device class structure
     */
    typedef void BR_ASM_CALL dev_load_cbfn(struct br_device *dev);
    typedef void BR_ASM_CALL dev_unload_cbfn(struct br_device *dev);
    typedef void BR_ASM_CALL dev_enquire_cbfn(struct br_device *dev);
    typedef void BR_ASM_CALL dev_create_cbfn(struct br_device *dev);

    typedef struct br_device
    {
        /*
         * Loaded devices are linked into a list
         */
        br_node node;

        char *identifier;

        dev_enquire_cbfn *enquire;
        dev_load_cbfn *load;
        dev_unload_cbfn *unload;
        dev_create_cbfn *create;

    } br_device;

/*
 * Given a pixelmap, returns the context to use
 *
 * XXX Handles NULL context pointer, but that should be sorted out in
 * PixelmapAllocate
 */
#define CONTEXT(pm) ((br_context *)(pm->context ? pm->context : &_BrMemoryContext))

#ifdef __cplusplus
};
#endif
#endif
