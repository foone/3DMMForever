/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: pmmemops.c 1.5 1995/08/31 16:29:41 sam Exp $
 * $Locker:  $
 *
 * Memeory Pixelmap operations
 */
#include <string.h>

#include "fw.h"
#include "brassert.h"

static char rscid[] = "$Id: pmmemops.c 1.5 1995/08/31 16:29:41 sam Exp $";

#define PMAP_ADDRESS(pm, x, y, bpp)                                                                                    \
    ((char *)((pm)->pixels) + ((pm)->base_y + (y)) * (pm)->row_bytes + ((pm)->base_x + (x)) * (bpp))

/*
 * Default device that just operates on normal memory pixelmaps
 */
br_device _BrMemoryDevice = {
    {0},
    "Memory Device",
};

br_context _BrMemoryContext = {
    "Memory Context",
    &_BrMemoryDevice,
    &_FontFixed3x5,

    _BrGenericFree,
    _BrGenericMatch,
    _BrGenericClone,
    _BrGenericDoubleBuffer,

    _BrPmMemCopy,
    _BrPmMemCopy,
    _BrPmMemCopy,

    _BrPmMemFill,
    _BrGenericRectangle,
    _BrGenericRectangle2,
    _BrPmMemRectangleCopyTo,
    _BrPmMemRectangleCopyTo,
    _BrPmMemRectangleCopyFrom,
    _BrPmMemRectangleFill,
    _BrPmMemDirtyRectangleCopy,
    _BrPmMemDirtyRectangleFill,
    _BrPmMemPixelSet,
    _BrPmMemPixelGet,
    _BrPmMemLine,
    _BrPmMemCopyBits,
};

void BR_ASM_CALL _BrPmMemFill(br_context *ctx, br_pixelmap *dst, br_uint_32 colour)
{
    int i;
    int bytes;
    char *dest;

    /*
     * normal pixelmap
     */
    bytes = BrPixelmapPixelSize(dst) >> 3;

    if ((dst->flags & (BR_PMF_LINEAR | BR_PMF_ROW_WHOLEPIXELS)) == (BR_PMF_LINEAR | BR_PMF_ROW_WHOLEPIXELS) &&
        dst->row_bytes > 0)
    {
        /*
         * Don't consider row ends - just blat the whole pixel block
         */
        dest = PMAP_ADDRESS(dst, 0, (dst->row_bytes > 0) ? 0 : (dst->height - 1), bytes);
        _MemFill_A(dest, dst->width * dst->height, bytes, colour);
    }
    else
    {
        dest = PMAP_ADDRESS(dst, 0, 0, bytes);
        _MemRectFill_A(dest, dst->width, dst->height, dst->row_bytes, bytes, colour);
    }
}

void BR_ASM_CALL _BrPmMemRectangleCopyTo(br_context *ctx, br_pixelmap *dst, br_uint_16 dx, br_uint_16 dy,
                                         br_pixelmap *src, br_uint_16 sx, br_uint_16 sy, br_uint_16 w, br_uint_16 h)
{
    int i, bytes;
    char *source, *dest;
    br_context *src_ctx;

    /*
     * Make sure the source is readable
     */
    UASSERT((src->flags & BR_PMF_NO_ACCESS) == 0);
    UASSERT(src->type == dst->type);

    src_ctx = CONTEXT(src);

    bytes = BrPixelmapPixelSize(src) >> 3;

    source = PMAP_ADDRESS(src, sx, sy, bytes);
    dest = PMAP_ADDRESS(dst, dx, dy, bytes);

    _MemRectCopy_A(dest, source, w, h, dst->row_bytes, src->row_bytes, bytes);
}

void BR_ASM_CALL _BrPmMemRectangleCopyFrom(br_context *ctx, br_pixelmap *dst, br_uint_16 dx, br_uint_16 dy,
                                           br_pixelmap *src, br_uint_16 sx, br_uint_16 sy, br_uint_16 w, br_uint_16 h)
{
    int i, bytes;
    char *source, *dest;
    br_context *dst_context;

    /*
     * Make sure the destination is writeable
     */
    UASSERT((dst->flags & BR_PMF_NO_ACCESS) == 0);
    UASSERT(src->type == dst->type);

    dst_context = CONTEXT(dst);

    bytes = BrPixelmapPixelSize(src) >> 3;

    source = PMAP_ADDRESS(src, sx, sy, bytes);
    dest = PMAP_ADDRESS(dst, dx, dy, bytes);

    _MemRectCopy_A(dest, source, w, h, dst->row_bytes, src->row_bytes, bytes);
}

void BR_ASM_CALL _BrPmMemRectangleFill(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y, br_uint_16 w,
                                       br_uint_16 h, br_uint_32 colour)
{
    int i, bytes, j;
    char *dest;

    bytes = BrPixelmapPixelSize(dst) >> 3;

    dest = PMAP_ADDRESS(dst, x, y, bytes);

    _MemRectFill_A(dest, w, h, dst->row_bytes, bytes, colour);
}

void BR_ASM_CALL _BrPmMemDirtyRectangleCopy(br_context *ctx, br_pixelmap *dst, br_pixelmap *src, br_uint_16 x,
                                            br_uint_16 y, br_uint_16 w, br_uint_16 h)
{
    _BrPmMemRectangleCopyTo(ctx, dst, x, y, src, x, y, w, h);
}

void BR_ASM_CALL _BrPmMemDirtyRectangleFill(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y, br_uint_16 w,
                                            br_uint_16 h, br_uint_32 colour)
{
    _BrPmMemRectangleFill(ctx, dst, x, y, w, h, colour);
}

void BR_ASM_CALL _BrPmMemCopy(br_context *ctx, br_pixelmap *dst, br_pixelmap *src)
{
    int i;
    int bytes;
    char *source, *dest;
    br_context *src_ctx;

    UASSERT((dst->type == src->type) && (dst->width == src->width) && (dst->height == src->height));
    UASSERT((src->flags & BR_PMF_NO_ACCESS) == 0);

    src_ctx = CONTEXT(src);

    bytes = BrPixelmapPixelSize(dst) >> 3;

    if ((dst->flags & (BR_PMF_LINEAR | BR_PMF_ROW_WHOLEPIXELS)) == (BR_PMF_LINEAR | BR_PMF_ROW_WHOLEPIXELS) &&
        (src->flags & (BR_PMF_LINEAR | BR_PMF_ROW_WHOLEPIXELS)) == (BR_PMF_LINEAR | BR_PMF_ROW_WHOLEPIXELS) &&
        dst->row_bytes > 0 && src->row_bytes > 0)
    {

        dest = PMAP_ADDRESS(dst, 0, (dst->row_bytes > 0) ? 0 : (dst->height - 1), bytes);
        source = PMAP_ADDRESS(src, 0, (dst->row_bytes > 0) ? 0 : (dst->height - 1), bytes);

        _MemCopy_A(dest, source, (dst->row_bytes * dst->height) / bytes, bytes);
    }
    else
    {
        dest = PMAP_ADDRESS(dst, 0, 0, bytes);
        source = PMAP_ADDRESS(src, 0, 0, bytes);

        _MemRectCopy_A(dest, source, dst->width, dst->height, dst->row_bytes, src->row_bytes, bytes);
    }
}

void BR_ASM_CALL _BrPmMemPixelSet(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y, br_uint_32 colour)
{
    int bytes;
    char *dest;

    /*
     * plot to normal pixelmap
     */
    bytes = BrPixelmapPixelSize(dst) >> 3;

    dest = PMAP_ADDRESS(dst, x, y, bytes);

    _MemPixelSet(dest, bytes, colour);
}

br_uint_32 BR_ASM_CALL _BrPmMemPixelGet(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y)
{
    int bytes;
    char *dest;

    /*
     * plot to normal pixelmap
     */
    bytes = BrPixelmapPixelSize(dst) >> 3;

    dest = PMAP_ADDRESS(dst, x, y, bytes);

    return _MemPixelGet(dest, bytes);
}

void BR_ASM_CALL _BrPmMemLine(br_context *ctx, br_pixelmap *dst, br_int_16 a1, br_int_16 b1, br_int_16 a2, br_int_16 b2,
                              br_uint_32 colour)
{
    /*
     * Symmetric Double Step Line Algorithm by Brian Wyvill from "Graphics Gems",
     * Academic Press, 1990
     */

#define LINEPLOT(x, y, flag)                                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        if (flag)                                                                                                      \
        {                                                                                                              \
            dest = PMAP_ADDRESS(dst, y, x, bytes);                                                                     \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            dest = PMAP_ADDRESS(dst, x, y, bytes);                                                                     \
        }                                                                                                              \
        _MemPixelSet(dest, bytes, colour);                                                                             \
    } while (0)

#define swap(a, b)                                                                                                     \
    {                                                                                                                  \
        a ^= b;                                                                                                        \
        b ^= a;                                                                                                        \
        a ^= b;                                                                                                        \
    }
#define absolute(i, j, k) ((i - j) * (k = ((i - j) < 0 ? -1 : 1)))

    int dx, dy, incr1, incr2, D, x, y, xend, c, pixels_left;
    int x1, y1;
    int sign_x, sign_y, step, reverse, i;
    char *dest;
    int bytes;

    bytes = BrPixelmapPixelSize(dst) >> 3;

    dx = absolute(a2, a1, sign_x);
    dy = absolute(b2, b1, sign_y);

    if (dx == 0 && dy == 0)
    {
        dest = PMAP_ADDRESS(dst, a1, b1, bytes);
        _MemPixelSet(dest, bytes, colour);
        return;
    };

    /* decide increment sign by the slope sign */
    if (sign_x == sign_y)
        step = 1;
    else
        step = -1;

    if (dy > dx)
    { /* chooses axis of greatest movement (make dx) */
        swap(a1, b1);
        swap(a2, b2);
        swap(dx, dy);
        reverse = 1;
    }
    else
        reverse = 0;

    /* note error check for dx==0 should be included here */
    if (a1 > a2)
    { /* start from the smaller coordinate */
        x = a2;
        y = b2;
        x1 = a1;
        y1 = b1;
    }
    else
    {
        x = a1;
        y = b1;
        x1 = a2;
        y1 = b2;
    }

    /* Note dx=n implies 0 - n or (dx+1) pixels to be set */
    /* Go round loop dx/4 times then plot last 0,1,2 or 3 pixels */
    /* In fact (dx-1)/4 as 2 pixels are already plottted */

    xend = (dx - 1) / 4;
    pixels_left = (dx - 1) % 4; /* number of pixels left over at the end */
    LINEPLOT(x, y, reverse);
    LINEPLOT(x1, y1, reverse); /* plot first two points */
    incr2 = 4 * dy - 2 * dx;
    if (incr2 < 0)
    { /* slope less than 1/2 */
        c = 2 * dy;
        incr1 = 2 * c;
        D = incr1 - dx;

        for (i = 0; i < xend; i++)
        { /* plotting loop */
            ++x;
            --x1;
            if (D < 0)
            {
                /* pattern 1 forwards */
                LINEPLOT(x, y, reverse);
                LINEPLOT(++x, y, reverse);
                /* pattern 1 backwards */
                LINEPLOT(x1, y1, reverse);
                LINEPLOT(--x1, y1, reverse);
                D += incr1;
            }
            else
            {
                if (D < c)
                {
                    /* pattern 2 forwards */
                    LINEPLOT(x, y, reverse);
                    LINEPLOT(++x, y += step, reverse);
                    /* pattern 2 backwards */
                    LINEPLOT(x1, y1, reverse);
                    LINEPLOT(--x1, y1 -= step, reverse);
                }
                else
                {
                    /* pattern 3 forwards */
                    LINEPLOT(x, y += step, reverse);
                    LINEPLOT(++x, y, reverse);
                    /* pattern 3 backwards */
                    LINEPLOT(x1, y1 -= step, reverse);
                    LINEPLOT(--x1, y1, reverse);
                }
                D += incr2;
            }
        } /* end for */

        /* plot last pattern */
        if (pixels_left)
        {
            if (D < 0)
            {
                LINEPLOT(++x, y, reverse); /* pattern 1 */
                if (pixels_left > 1)
                    LINEPLOT(++x, y, reverse);
                if (pixels_left > 2)
                    LINEPLOT(--x1, y1, reverse);
            }
            else
            {
                if (D < c)
                {
                    LINEPLOT(++x, y, reverse); /* pattern 2  */
                    if (pixels_left > 1)
                        LINEPLOT(++x, y += step, reverse);
                    if (pixels_left > 2)
                        LINEPLOT(--x1, y1, reverse);
                }
                else
                {
                    /* pattern 3 */
                    LINEPLOT(++x, y += step, reverse);
                    if (pixels_left > 1)
                        LINEPLOT(++x, y, reverse);
                    if (pixels_left > 2)
                        LINEPLOT(--x1, y1 -= step, reverse);
                }
            }
        } /* end if pixels_left */
    }
    /* end slope < 1/2 */
    else
    { /* slope greater than 1/2 */
        c = 2 * (dy - dx);
        incr1 = 2 * c;
        D = incr1 + dx;
        for (i = 0; i < xend; i++)
        {
            ++x;
            --x1;
            if (D > 0)
            {
                /* pattern 4 forwards */
                LINEPLOT(x, y += step, reverse);
                LINEPLOT(++x, y += step, reverse);
                /* pattern 4 backwards */
                LINEPLOT(x1, y1 -= step, reverse);
                LINEPLOT(--x1, y1 -= step, reverse);
                D += incr1;
            }
            else
            {
                if (D < c)
                {
                    /* pattern 2 forwards */
                    LINEPLOT(x, y, reverse);
                    LINEPLOT(++x, y += step, reverse);

                    /* pattern 2 backwards */
                    LINEPLOT(x1, y1, reverse);
                    LINEPLOT(--x1, y1 -= step, reverse);
                }
                else
                {
                    /* pattern 3 forwards */
                    LINEPLOT(x, y += step, reverse);
                    LINEPLOT(++x, y, reverse);
                    /* pattern 3 backwards */
                    LINEPLOT(x1, y1 -= step, reverse);
                    LINEPLOT(--x1, y1, reverse);
                }
                D += incr2;
            }
        } /* end for */
        /* plot last pattern */
        if (pixels_left)
        {
            if (D > 0)
            {
                LINEPLOT(++x, y += step, reverse); /* pattern 4 */
                if (pixels_left > 1)
                    LINEPLOT(++x, y += step, reverse);
                if (pixels_left > 2)
                    LINEPLOT(--x1, y1 -= step, reverse);
            }
            else
            {
                if (D < c)
                {
                    LINEPLOT(++x, y, reverse); /* pattern 2  */
                    if (pixels_left > 1)
                        LINEPLOT(++x, y += step, reverse);
                    if (pixels_left > 2)
                        LINEPLOT(--x1, y1, reverse);
                }
                else
                {
                    /* pattern 3 */
                    LINEPLOT(++x, y += step, reverse);
                    if (pixels_left > 1)
                        LINEPLOT(++x, y, reverse);
                    if (pixels_left > 2)
                    {
                        if (D > c) /* step 3 */
                            LINEPLOT(--x1, y1 -= step, reverse);
                        else /* step 2 */
                            LINEPLOT(--x1, y1, reverse);
                    }
                }
            }
        }
    }
}

void BR_ASM_CALL _BrPmMemCopyBits(br_context *ctx, br_pixelmap *dst, br_int_16 x, br_int_16 y, br_uint_8 *src,
                                  br_uint_16 s_stride, br_uint_16 start_bit, br_uint_16 end_bit, br_uint_16 nrows,
                                  br_uint_32 colour)
{
    int bytes = BrPixelmapPixelSize(dst) >> 3;

    _MemCopyBits_A(PMAP_ADDRESS(dst, x, y, bytes), dst->row_bytes, src, s_stride, start_bit, end_bit, nrows, bytes,
                   colour);
}
