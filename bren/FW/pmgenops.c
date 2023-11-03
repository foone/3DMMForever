/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: pmgenops.c 1.6 1995/08/31 16:29:39 sam Exp $
 * $Locker:  $
 *
 * Generic Pixelmap operations
 */
#include <string.h>

#include "fw.h"
#include "brassert.h"

static char rscid[] = "$Id: pmgenops.c 1.6 1995/08/31 16:29:39 sam Exp $";

#define PMAP_ADDRESS(pm, x, y, bpp)                                                                                    \
    ((char *)((pm)->pixels) + ((pm)->base_y + (y)) * (pm)->row_bytes + ((pm)->base_x + (x)) * (bpp))

void BR_ASM_CALL _BrGenericLine(br_context *ctx, br_pixelmap *dst, br_int_16 a1, br_int_16 b1, br_int_16 a2,
                                br_int_16 b2, br_uint_32 colour)
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
            ctx->pixel_set(ctx, dst, y, x, colour);                                                                    \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            ctx->pixel_set(ctx, dst, x, y, colour);                                                                    \
        }                                                                                                              \
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
        ctx->pixel_set(ctx, dst, a1, b1, colour);
        return;
    }

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

void BR_ASM_CALL _BrGenericDoubleBuffer(br_context *ctx, br_pixelmap *dest, br_pixelmap *src)
{
    ctx->copy_to(ctx, dest, src);
}

br_pixelmap *BR_ASM_CALL _BrGenericMatch(br_context *ctx, br_pixelmap *src, br_uint_8 match_type)
{
    int bytes, row_bytes;
    br_pixelmap *pm = NULL;

    UASSERT(match_type < BR_PMMATCH_MAX);

    switch (match_type)
    {
    case BR_PMMATCH_OFFSCREEN:
        /*
         * Clone the source buffer
         */
        pm = BrPixelmapAllocate(src->type, src->width, src->height, NULL, (src->row_bytes < 0) ? BR_PMAF_INVERTED : 0);
        break;

    case BR_PMMATCH_DEPTH_16:
        /*
         * Allocate a DEPTH_16 pixelmap that has it's row_bytes as a multiple of the src row_bytes
         */
        pm = BrPixelmapAllocate(BR_PMT_DEPTH_16, src->width, src->height, (void *)src, 0);
        bytes = BrPixelmapPixelSize(src) >> 3;

        if (bytes != 0)
            pm->row_bytes = (src->row_bytes * 2) / bytes;
        else
            pm->row_bytes = (src->row_bytes * 2);

        row_bytes = (pm->row_bytes > 0) ? pm->row_bytes : -pm->row_bytes;
        pm->pixels = BrResAllocate(pm, row_bytes * pm->height, BR_MEMORY_PIXELS);

        if (pm->row_bytes < 0)
            pm->pixels = (char *)pm->pixels + row_bytes * (pm->height - 1);

        break;
    }

    return pm;
}

/*
 * Make a copy of a pixelmap
 */
br_pixelmap *BR_ASM_CALL _BrGenericClone(br_context *ctx, br_pixelmap *src)
{
    br_pixelmap *pm;

    pm = BrPixelmapAllocate(src->type, src->width, src->height, NULL, (src->row_bytes < 0) ? BR_PMAF_INVERTED : 0);

    BrPixelmapCopy(pm, src);

    return pm;
}

/*
 * Relase a pixelmap and all its associated resources
 */
void BR_ASM_CALL _BrGenericFree(br_context *ctx, br_pixelmap *src)
{
    BrResFree(src);
}

void BR_ASM_CALL _BrGenericDirtyRectangleCopy(br_context *ctx, br_pixelmap *dst, br_pixelmap *src, br_uint_16 x,
                                              br_uint_16 y, br_uint_16 w, br_uint_16 h)
{
    ctx->rectangle_copy(ctx, dst, x, y, src, x, y, w, h);
}

void BR_ASM_CALL _BrGenericDirtyRectangleFill(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y,
                                              br_uint_16 w, br_uint_16 h, br_uint_32 colour)
{
    ctx->rectangle_fill(ctx, dst, x, y, w, h, colour);
}

/*
 * These should be moved into FW
 */
void BR_ASM_CALL _BrGenericRectangle(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y, br_uint_16 w,
                                     br_uint_16 h, br_uint_32 colour)
{
    int x1, y1;

    x1 = x + w - 1;
    y1 = y + h - 1;

    ctx->line(ctx, dst, x, y, x1, y, colour);
    ctx->line(ctx, dst, x, y, x, y1, colour);

    ctx->line(ctx, dst, x1, y1, x1, y, colour);
    ctx->line(ctx, dst, x1, y1, x, y1, colour);
}

void BR_ASM_CALL _BrGenericRectangle2(br_context *ctx, br_pixelmap *dst, br_uint_16 x, br_uint_16 y, br_uint_16 w,
                                      br_uint_16 h, br_uint_32 colour1, br_uint_32 colour2)
{
    int x1, y1;

    x1 = x + w - 1;
    y1 = y + h - 1;

    ctx->line(ctx, dst, x, y, x1, y, colour1);
    ctx->line(ctx, dst, x, y, x, y1, colour1);

    ctx->line(ctx, dst, x1, y1, x1, y, colour2);
    ctx->line(ctx, dst, x1, y1, x, y1, colour2);
}

void BR_ASM_CALL _BrGenericRectangleCopy(br_context *ctx, br_pixelmap *dst, br_uint_16 dx, br_uint_16 dy,
                                         br_pixelmap *src, br_uint_16 sx, br_uint_16 sy, br_uint_16 w, br_uint_16 h)
{
    /* XXX */

    /*
     * Use a fixed size mmeory buffer (4K ?)
     *
     * Copy a rectangle at a time
     * 		from source to memory
     * 		from memory to destination
     */
}

void BR_ASM_CALL _BrGenericCopy(br_context *ctx, br_pixelmap *dst, br_pixelmap *src)
{
    /* XXX */
    /* Use RectangleCopy */
}
