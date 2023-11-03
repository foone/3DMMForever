/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: p_piz.c 1.1 1995/08/31 16:52:49 sam Exp $
 * $Locker:  $
 *
 * Mesh rendering to produce points
 */
#include "zb.h"
#include "shortcut.h"
#include "blockops.h"

static char rscid[] = "$Id: p_piz.c 1.1 1995/08/31 16:52:49 sam Exp $";

#define ScalarsToRGB15(r, g, b) (((r) >> 19) & 0x1f | ((g) >> 14) & 0x3e0 | ((b) >> 9) & 0x7c00)

#define ScalarsToRGB16(r, g, b) (((r) >> 19) & 0x1f | ((g) >> 14) & 0x7c0 | ((b) >> 8) & 0xf800)

#define ScalarsToRGB24(r, g, b) (((b) >> 16) & 0xff | ((g) >> 8) & 0xff00 | ((b) >> 8) & 0xff0000)

#define SETUP_POINT                                                                                                    \
    x0 = ScreenToInt(tvp->v[X]);                                                                                       \
    y0 = ScreenToInt(tvp->v[Y]);                                                                                       \
    CLAMP_LP(x0, y0);

#define SETUP_TEXTURE                                                                                                  \
    width = zb.material->colour_map->width;                                                                            \
    height = zb.material->colour_map->height;                                                                          \
    stride = zb.material->colour_map->row_bytes;

/*
 * Setup arbitrary width u and v
 */
#define SETUP_UV                                                                                                       \
    pu = tvp->comp[C_U] % BrIntToFixed(width);                                                                         \
    if (pu < 0)                                                                                                        \
        pu += BrIntToFixed(width);                                                                                     \
    pv = tvp->comp[C_V] % BrIntToFixed(height);                                                                        \
    if (pv < 0)                                                                                                        \
        pv += BrIntToFixed(height);

/*
 * Set up RGB
 */
#define SETUP_RGB                                                                                                      \
    pr = tvp->comp[C_R];                                                                                               \
    pg = tvp->comp[C_G];                                                                                               \
    pb = tvp->comp[C_B];

/*
 * 8 bit z-buffered points
 */
void BR_ASM_CALL PointRenderPIZ2(struct temp_vertex_fixed *tvp)
{
    int o;
    int x0, y0;

    SETUP_POINT;

    o = y0 * zb.row_width + x0;

    if (((unsigned short *)zb.depth_buffer)[o] > (unsigned short)(tvp->v[Z] >> 16))
    {
        ((unsigned short *)zb.depth_buffer)[o] = (unsigned short)(tvp->v[Z] >> 16);
        zb.colour_buffer[o] = BrFixedToInt(tvp->comp[C_I]);
    }
}

void BR_ASM_CALL PointRenderPIZ2TI(struct temp_vertex_fixed *tvp)
{
    int o;
    int x0, y0;
    br_fixed_ls width, height, stride, pi, pu, pv;
    char texel;

    SETUP_POINT;
    SETUP_TEXTURE;
    SETUP_UV;

    pi = tvp->comp[C_I];

    o = y0 * zb.row_width + x0;

    if (((unsigned short *)zb.depth_buffer)[o] > (unsigned short)(tvp->v[Z] >> 16))
    {
        texel = zb.texture_buffer[(pv >> 16) * width + (pu >> 16)];
        if (texel)
        {
            ((unsigned short *)zb.depth_buffer)[o] = (unsigned short)(tvp->v[Z] >> 16);
            zb.colour_buffer[o] = zb.shade_table[256 * (pi >> 16) + texel];
        }
    }
}

/*
 * 8 bit textured points
 */
void BR_ASM_CALL PointRenderPIZ2T(struct temp_vertex_fixed *tvp)
{
    int o;
    int x0, y0;
    br_fixed_ls width, height, stride, pu, pv;
    char texel;

    SETUP_POINT;
    SETUP_TEXTURE;
    SETUP_UV;

    o = y0 * zb.row_width + x0;

    if (((unsigned short *)zb.depth_buffer)[o] > (unsigned short)(tvp->v[Z] >> 16))
    {
        texel = zb.texture_buffer[(pv >> 16) * stride + (pu >> 16)];
        if (texel)
        {
            ((unsigned short *)zb.depth_buffer)[o] = (unsigned short)(tvp->v[Z] >> 16);
            zb.colour_buffer[o] = texel;
        }
    }
}

typedef enum
{
    LINE_RGB,
    LINE_TEXTURE
} point_type;

/*
 * 24 bit point renderer
 */

void BR_ASM_CALL PointRenderPIZ2_Generic_RGB_888(point_type mode, struct temp_vertex_fixed *tvp)
{
    int oc, oz;
    int x0, y0;
    br_fixed_ls width, height, stride, pi, pu, pv, pr, pg, pb;
    char *texel;

    SETUP_POINT;

    if (mode == LINE_RGB)
    {
        SETUP_RGB;
    }

    if (mode == LINE_TEXTURE)
    {

        /* Textured */

        SETUP_TEXTURE;
        SETUP_UV;
    }

    oc = y0 * zb.row_width + 3 * x0;
    oz = y0 * zb.depth_row_width + x0 + x0;

    if (*(unsigned short *)((char *)zb.depth_buffer + oz) > (unsigned short)(tvp->v[Z] >> 16))
    {
        if (mode == LINE_TEXTURE)
        {
            texel = zb.texture_buffer + (pv >> 16) * stride + 3 * (pu >> 16);
            if (texel[0] || texel[1] || texel[2])
            {
                *(unsigned short *)((char *)zb.depth_buffer + oz) = (unsigned short)(tvp->v[Z] >> 16);
                zb.colour_buffer[oc] = texel[0];
                zb.colour_buffer[oc + 1] = texel[1];
                zb.colour_buffer[oc + 2] = texel[2];
            }
        }
        else
        {
            *(unsigned short *)((char *)zb.depth_buffer + oz) = (unsigned short)(tvp->v[Z] >> 16);
            zb.colour_buffer[oc] = pb >> 16;
            zb.colour_buffer[oc + 1] = pg >> 16;
            zb.colour_buffer[oc + 2] = pr >> 16;
        }
    }
}

/*
 * 24 bit coloured points
 */

void BR_ASM_CALL PointRenderPIZ2_RGB_888(struct temp_vertex_fixed *tvp)
{
    PointRenderPIZ2_Generic_RGB_888(LINE_RGB, tvp);
}

/*
 * 24 bit textured points
 */

void BR_ASM_CALL PointRenderPIZ2T_RGB_888(struct temp_vertex_fixed *tvp)
{
    PointRenderPIZ2_Generic_RGB_888(LINE_TEXTURE, tvp);
}

/*
 * 24 bit points
 */

void BR_ASM_CALL PointRenderPIZ2_Generic_RGB_555(point_type mode, struct temp_vertex_fixed *tvp)
{
    int oc, oz;
    int x0, y0;
    br_fixed_ls width, height, stride, pi, pu, pv, pr, pg, pb;
    unsigned short texel;

    SETUP_POINT;

    if (mode == LINE_RGB)
    {
        SETUP_RGB;
    }

    if (mode == LINE_TEXTURE)
    {

        /* Textured */

        SETUP_TEXTURE;
        SETUP_UV;
    }

    oc = y0 * zb.row_width + 2 * x0;
    oz = y0 * zb.depth_row_width + x0 + x0;

    if (*(unsigned short *)((char *)zb.depth_buffer + oz) > (unsigned short)(tvp->v[Z] >> 16))
    {
        if (mode == LINE_TEXTURE)
        {
            texel = *(unsigned short *)(zb.texture_buffer + (pv >> 16) * stride + 2 * (pu >> 16));
            if (texel)
            {
                *(unsigned short *)((char *)zb.depth_buffer + oz) = (unsigned short)(tvp->v[Z] >> 16);
                *(unsigned short *)(zb.colour_buffer + oc) = texel;
            }
        }
        else
        {
            *(unsigned short *)((char *)zb.depth_buffer + oz) = (unsigned short)(tvp->v[Z] >> 16);
            *(unsigned short *)(zb.colour_buffer + oc) = ScalarsToRGB15(pr, pg, pb);
        }
    }
}

/*
 * 24 bit coloured points
 */
void BR_ASM_CALL PointRenderPIZ2_RGB_555(struct temp_vertex_fixed *tvp)
{
    PointRenderPIZ2_Generic_RGB_555(LINE_RGB, tvp);
}

/*
 * 24 bit textured points
 */
void BR_ASM_CALL PointRenderPIZ2T_RGB_555(struct temp_vertex_fixed *tvp)
{
    PointRenderPIZ2_Generic_RGB_555(LINE_TEXTURE, tvp);
}

