/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: pmdsptch.c 1.6 1995/08/31 16:29:38 sam Exp $
 * $Locker: sam $
 *
 * Pixelmap operations
 */
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "fw.h"
#include "brassert.h"

static char rscid[] = "$Id: pmdsptch.c 1.6 1995/08/31 16:29:38 sam Exp $";

void BR_PUBLIC_ENTRY BrPixelmapFree(br_pixelmap *src)
{
    br_context *ctx = CONTEXT(src);

    ctx->free(ctx, src);
}

br_pixelmap *BR_PUBLIC_ENTRY BrPixelmapMatch(br_pixelmap *src, br_uint_8 match_type)
{
    br_context *ctx = CONTEXT(src);

    return ctx->match(ctx, src, match_type);
}

br_pixelmap *BR_PUBLIC_ENTRY BrPixelmapClone(br_pixelmap *src)
{
    br_context *ctx = CONTEXT(src);

    return ctx->clone(ctx, src);
}

void BR_PUBLIC_ENTRY BrPixelmapFill(br_pixelmap *dst, br_uint_32 colour)
{
    br_context *ctx = CONTEXT(dst);

    ctx->fill(ctx, dst, colour);
}

void BR_PUBLIC_ENTRY BrPixelmapRectangle(br_pixelmap *dst, br_int_16 x, br_int_16 y, br_uint_16 w, br_uint_16 h,
                                         br_uint_32 colour)
{
    br_context *ctx = CONTEXT(dst);

    x += dst->origin_x;
    y += dst->origin_y;

    ctx->rectangle(ctx, dst, x, y, w, h, colour);
}

void BR_PUBLIC_ENTRY BrPixelmapRectangle2(br_pixelmap *dst, br_int_16 x, br_int_16 y, br_uint_16 w, br_uint_16 h,
                                          br_uint_32 colour1, br_uint_32 colour2)
{
    br_context *ctx = CONTEXT(dst);

    x += dst->origin_x;
    y += dst->origin_y;

    ctx->rectangle2(ctx, dst, x, y, w, h, colour1, colour2);
}

void BR_PUBLIC_ENTRY BrPixelmapRectangleCopy(br_pixelmap *dst, br_int_16 dx, br_int_16 dy, br_pixelmap *src,
                                             br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h)
{
    br_context *s_ctx = CONTEXT(src);
    br_context *d_ctx = CONTEXT(dst);

    sx += src->origin_x;
    sy += src->origin_y;

    dx += dst->origin_x;
    dy += dst->origin_y;

    /*
     * Trivial reject
     */
    if (dx >= dst->width || dy >= dst->height)
        return;

    if (sx >= src->width || sy >= src->height)
        return;

    if ((dx + w) <= 0 || (dy + h) <= 0)
        return;

    if ((sx + w) <= 0 || (sy + h) <= 0)
        return;

    /*
     * Clip rectangle to destination
     */
    if (dx < 0)
    {
        w += dx;
        sx -= dx;
        dx = 0;
    }

    if (dy < 0)
    {
        h += dy;
        sy -= dy;
        dy = 0;
    }

    if ((dx + w) > dst->width)
        w = dst->width - dx;

    if ((dy + h) > dst->height)
        h = dst->height - dy;

    /*
     * Clip rectangle to source
     */
    if (sx < 0)
    {
        w += sx;
        dx -= sx;
        sx = 0;
    }

    if (sy < 0)
    {
        h += sy;
        dy -= sy;
        sy = 0;
    }

    if ((sx + w) > src->width)
        w = src->width - sx;

    if ((sy + h) > src->height)
        h = src->height - sy;

    /*
     * Don't draw empty rectangles
     */
    if (w == 0 || h == 0)
        return;

    /*
     * Invoke one of three driver functions
     *	device -> device
     *	device -> memory
     *	memory -> device
     *	or Generic handler
     */
    if (s_ctx->device == d_ctx->device)
        /*
         * If the same device, just let the device get on with it
         */
        d_ctx->rectangle_copy(d_ctx, dst, dx, dy, src, sx, sy, w, h);
    else if (s_ctx->device == &_BrMemoryDevice)
        /*
         * Spot case of copying to device (from memory)
         */
        d_ctx->rectangle_copy_to(d_ctx, dst, dx, dy, src, sx, sy, w, h);
    else if (d_ctx->device == &_BrMemoryDevice)
        /*
         * Spot case of copying from device (to memory)
         */
        s_ctx->rectangle_copy_from(s_ctx, dst, dx, dy, src, sx, sy, w, h);
    else
        /*
         * Otherwise use a general copy with intermediate buffer
         */
        _BrGenericRectangleCopy(d_ctx, dst, dx, dy, src, sx, sy, w, h);
}

void BR_PUBLIC_ENTRY BrPixelmapRectangleFill(br_pixelmap *dst, br_int_16 x, br_int_16 y, br_uint_16 w, br_uint_16 h,
                                             br_uint_32 colour)
{
    br_context *ctx = CONTEXT(dst);

    x += dst->origin_x;
    y += dst->origin_y;

    /*
     * Trivial reject
     */
    if (x >= dst->width || y >= dst->height)
        return;

    if ((x + w) <= 0 || (y + h) <= 0)
        return;

    /*
     * Clip rectangle to destination
     */

    if (x < 0)
    {
        w += x;
        x = 0;
    }

    if (y < 0)
    {
        h += y;
        y = 0;
    }

    if ((x + w) > dst->width)
        w = dst->width - x;

    if ((y + h) > dst->height)
        h = dst->height - y;

    /*
     * Don't draw empty rectangles
     */
    if (w == 0 || h == 0)
        return;

    ctx->rectangle_fill(ctx, dst, x, y, w, h, colour);
}

void BR_PUBLIC_ENTRY BrPixelmapDirtyRectangleCopy(br_pixelmap *dst, br_pixelmap *src, br_int_16 x, br_int_16 y,
                                                  br_uint_16 w, br_uint_16 h)
{
    br_context *ctx = CONTEXT(dst);

    x += dst->origin_x;
    y += dst->origin_y;

    UASSERT(src->type == dst->type);
    UASSERT(src->width == dst->width);
    UASSERT(src->height == dst->height);
    UASSERT(src->row_bytes == dst->row_bytes);

    /*
     * Trivial reject
     */
    if (x >= dst->width || y >= dst->height)
        return;

    if ((x + w) <= 0 || (y + h) <= 0)
        return;

    /*
     * Clip rectangle to destination
     */
    if (x < 0)
    {
        w += x;
        x = 0;
    }

    if (y < 0)
    {
        h += y;
        y = 0;
    }

    if ((x + w) > dst->width)
        w = dst->width - x;

    if ((y + h) > dst->height)
        h = dst->height - y;

    /*
     * Don't draw empty rectangles
     */
    if (w == 0 || h == 0)
        return;

    ctx->dirty_rectangle_copy(ctx, dst, src, x, y, w, h);
}

void BR_PUBLIC_ENTRY BrPixelmapDirtyRectangleFill(br_pixelmap *dst, br_int_16 x, br_int_16 y, br_uint_16 w,
                                                  br_uint_16 h, br_uint_32 colour)
{
    br_context *ctx = CONTEXT(dst);

    x += dst->origin_x;
    y += dst->origin_y;

    /*
     * Trivial reject
     */
    if (x >= dst->width || y >= dst->height)
        return;

    if ((x + w) <= 0 || (y + h) <= 0)
        return;

    /*
     * Clip rectangle to destination
     */
    if (x < 0)
    {
        w += x;
        x = 0;
    }

    if (y < 0)
    {
        h += y;
        y = 0;
    }

    if ((x + w) > dst->width)
        w = dst->width - x;

    if ((y + h) > dst->height)
        h = dst->height - y;

    /*
     * Don't draw empty rectangles
     */
    if (w == 0 || h == 0)
        return;

    ctx->dirty_rectangle_fill(ctx, dst, x, y, w, h, colour);
}

void BR_PUBLIC_ENTRY BrPixelmapPixelSet(br_pixelmap *dst, br_int_16 x, br_int_16 y, br_uint_32 colour)
{
    br_context *ctx = CONTEXT(dst);

    x += dst->origin_x;
    y += dst->origin_y;

    if (x < 0 || y < 0)
        return;

    if (x >= dst->width || y >= dst->height)
        return;

    ctx->pixel_set(ctx, dst, x, y, colour);
}

br_uint_32 BR_PUBLIC_ENTRY BrPixelmapPixelGet(br_pixelmap *dst, br_int_16 x, br_int_16 y)
{
    br_context *ctx = CONTEXT(dst);

    x += dst->origin_x;
    y += dst->origin_y;

    if (x < 0 || y < 0)
        return 0;

    if (x >= dst->width || y >= dst->height)
        return 0;

    return ctx->pixel_get(ctx, dst, x, y);
}

void BR_PUBLIC_ENTRY BrPixelmapCopy(br_pixelmap *dst, br_pixelmap *src)
{
    br_context *s_ctx = CONTEXT(src);
    br_context *d_ctx = CONTEXT(dst);

    UASSERT((dst->type == src->type) && (dst->width == src->width) && (dst->height == src->height));
    UASSERT((src->flags & BR_PMF_NO_ACCESS) == 0);

    /*
     * Invoke one of three driver functions
     *	device -> device
     *	device -> memory
     *	memory -> device
     *	or Generic handler
     */
    if (s_ctx->device == d_ctx->device)
        /*
         * If the same device, just let the device get on with it
         */
        d_ctx->copy(d_ctx, dst, src);
    else if (s_ctx->device == &_BrMemoryDevice)
        /*
         * Spot case of copying to device (from memory)
         */
        d_ctx->copy_to(d_ctx, dst, src);
    else if (d_ctx->device == &_BrMemoryDevice)
        /*
         * Spot case of copying from device (to memory)
         */
        s_ctx->copy_from(s_ctx, dst, src);
    else
        /*
         * Otherwise use a general copy with intermediate buffer
         */
        _BrGenericCopy(d_ctx, dst, src);
}

void BR_PUBLIC_ENTRY BrPixelmapLine(br_pixelmap *dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2,
                                    br_uint_32 colour)
{
    int temp;
    br_int_16 w, h;
    br_context *ctx = CONTEXT(dst);

    x1 += dst->origin_x;
    y1 += dst->origin_y;
    x2 += dst->origin_x;
    y2 += dst->origin_y;

#define SCALE(var, arg, num, den) ((var) = ((arg) * (num)) / (den))

#define USCALE(var, arg, num, den) ((var) = ((unsigned)(arg) * (unsigned)(num)) / (unsigned)(den))

#define EXCHG(a, b)                                                                                                    \
    do                                                                                                                 \
    {                                                                                                                  \
        int __temp__ = (a);                                                                                            \
        (a) = (b);                                                                                                     \
        (b) = __temp__;                                                                                                \
    } while (0)

#define WHEN_CLIPPED

    w = dst->width - 1;
    h = dst->height - 1;

    if (x1 > x2)
    {
        EXCHG(x1, x2);
        EXCHG(y1, y2);
    }

    if ((x2 < 0) || (x1 > w))
    {
        return;
    }

    if (y1 < y2)
    {
        if ((y2 < 0) || (y1 > h))
        {
            return;
        }
        if (y1 < 0)
        {
            USCALE(temp, (x2 - x1), (0 - y1), (y2 - y1));
            if ((x1 += temp) > w)
            {
                return;
            }
            y1 = 0;
            WHEN_CLIPPED;
        }
        if (y2 > h)
        {
            USCALE(temp, (x2 - x1), (y2 - h), (y2 - y1));
            if ((x2 -= temp) < 0)
            {
                return;
            }
            y2 = h;
            WHEN_CLIPPED;
        }
        if (x1 < 0)
        {
            USCALE(temp, (y2 - y1), (0 - x1), (x2 - x1));
            y1 += temp;
            x1 = 0;
            WHEN_CLIPPED;
        }
        if (x2 > w)
        {
            USCALE(temp, (y2 - y1), (x2 - w), (x2 - x1));
            y2 -= temp;
            x2 = w;
            WHEN_CLIPPED;
        }
    }
    else
    {
        if ((y1 < 0) || (y2 > h))
        {
            return;
        }
        if (y1 > h)
        {
            USCALE(temp, (x2 - x1), (y1 - h), (y1 - y2));
            if ((x1 += temp) > w)
            {
                return;
            }
            y1 = h;
            WHEN_CLIPPED;
        }
        if (y2 < 0)
        {
            USCALE(temp, (x2 - x1), (0 - y2), (y1 - y2));
            if ((x2 -= temp) < 0)
            {
                return;
            }
            y2 = 0;
            WHEN_CLIPPED;
        }
        if (x1 < 0)
        {
            USCALE(temp, (y1 - y2), (0 - x1), (x2 - x1));
            y1 -= temp;
            x1 = 0;
            WHEN_CLIPPED;
        }
        if (x2 > w)
        {
            USCALE(temp, (y1 - y2), (x2 - w), (x2 - x1));
            y2 += temp;
            x2 = w;
            WHEN_CLIPPED;
        }
    }

    ctx->line(ctx, dst, x1, y1, x2, y2, colour);
}

br_pixelmap *BR_PUBLIC_ENTRY BrPixelmapDoubleBuffer(br_pixelmap *dst, br_pixelmap *src)
{
    br_context *ctx = CONTEXT(dst);

    ctx->double_buffer(ctx, dst, src);

    return src;
}

void BR_PUBLIC_ENTRY BrPixelmapText(br_pixelmap *dst, br_int_16 x, br_int_16 y, br_uint_32 colour, br_font *font,
                                    char *text)
{
    int w, s_stride, c;
    int rows, base;
    int bytes;
    br_context *ctx = CONTEXT(dst);

    x += dst->origin_x;
    y += dst->origin_y;

    if (font == NULL)
        font = ctx->font;

    /*
     * Quit if off top, bottom or right screen
     */
    if (y <= -font->glyph_y || y >= dst->height || x >= dst->width)
        return;

    /*
     * Clip to bottom of screen
     */
    if (y + font->glyph_y > dst->height)
        rows = dst->height - y;
    else
        rows = font->glyph_y;

    /*
     * Clip to top of screen
     */
    if (y < 0)
    {
        base = -y;
        rows -= base;
        y = 0;
    }
    else
    {
        base = 0;
    }

    /*
     * Remaining space until right edge
     */
    c = dst->width - x;

    if (font->flags & BR_FONTF_PROPORTIONAL)
    {
        /*
         * PROPORTIONAL
         */
        for (; *text; text++)
        {
            w = font->width[*text];
            s_stride = (w + 7) / 8;

            if (x + w > 0)
            {
                ctx->copy_bits(ctx, dst, x, y, font->glyphs + font->encoding[*text] + base * s_stride, s_stride,
                               (x < 0) ? -x : 0, (w < c) ? w : c, rows, colour);
            }
            x += w + 1;
            c -= w + 1;

            if (c <= 0)
                break;
        }
    }
    else
    {
        /*
         * FIXED
         */
        w = font->glyph_x;
        s_stride = (w + 7) / 8;

        for (; *text; text++)
        {

            if (x + w > 0)
            {
                ctx->copy_bits(ctx, dst, x, y, font->glyphs + font->encoding[*text] + base * s_stride, s_stride,
                               (x < 0) ? -x : 0, (w < c) ? w : c, rows, colour);
            }
            x += w + 1;
            c -= w + 1;

            if (c <= 0)
                break;
        }
    }
}

void BR_PUBLIC_ENTRY BrPixelmapTextF(br_pixelmap *dst, br_int_16 x, br_int_16 y, br_uint_32 colour, br_font *font,
                                     char *fmt, ...)
{
    va_list args;

    /*
     * Build output string
     */
    va_start(args, fmt);
    vsprintf(_br_scratch_string, fmt, args);
    va_end(args);

    BrPixelmapText(dst, x, y, colour, font, _br_scratch_string);
}

br_uint_16 BR_PUBLIC_ENTRY BrPixelmapTextWidth(br_pixelmap *dst, br_font *font, char *text)
{
    int i, j, w;
    br_context *ctx = CONTEXT(dst);

    if (font == NULL)
        font = ctx->font;

    if (font->flags & BR_FONTF_PROPORTIONAL)
    {
        /*
         * return length of (proportional) string in pixels
         */
        for (i = 0, w = 0, j = strlen(text); i < j; i++, text++)
            w += font->width[*text] + 1;

        w -= 1;
        return w;
    }
    else
        return (font->glyph_x + 1) * strlen(text) - 1;
}

br_uint_16 BR_PUBLIC_ENTRY BrPixelmapTextHeight(br_pixelmap *dst, br_font *font)
{
    br_context *ctx = CONTEXT(dst);

    if (font == NULL)
        font = ctx->font;

    return font->glyph_y;
}

void BR_PUBLIC_ENTRY BrPixelmapCopyBits(br_pixelmap *dst, br_int_16 x, br_int_16 y, br_uint_8 *src, br_uint_16 s_stride,
                                        br_uint_16 start_bit, br_uint_16 end_bit, br_uint_16 nrows, br_uint_32 colour)
{
    br_context *ctx = CONTEXT(dst);

    x += dst->origin_x;
    y += dst->origin_y;

    /*
     * Trivial reject
     */
    if (x + start_bit >= dst->width)
        return;
    if (y >= dst->height)
        return;

    if (x + end_bit <= 0)
        return;
    if (y + nrows <= 0)
        return;

    /*
     * Clip to destination
     */
    if (x + end_bit > dst->width)
        end_bit = dst->width - x;

    if (x + start_bit < 0)
        start_bit = -x;

    if ((y + nrows) > dst->height)
        nrows = dst->height - y;

    if (y < 0)
    {
        src -= y * s_stride;
        nrows += y;
        y = 0;
    }

    if (end_bit < start_bit)
        return;

    ctx->copy_bits(ctx, dst, x, y, src, s_stride, start_bit, end_bit, nrows, colour);
}
