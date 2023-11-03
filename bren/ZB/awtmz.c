/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: awtmz.c 1.1 1995/08/31 16:52:48 sam Exp $
 * $Locker:  $
 *
 * Arbitary width texture mapper
 */
#include <math.h>

#include <brbuiltin.h>
#include <brbithax.h>

#include "brender.h"
#include "zb.h"
#include "shortcut.h"
#include "brassert.h"

#define swap(type, a, b)                                                                                               \
    {                                                                                                                  \
        type _;                                                                                                        \
        _ = a;                                                                                                         \
        a = b;                                                                                                         \
        b = _;                                                                                                         \
    }

#define high16(a) ((br_uint_32)(a) >> 16)
#define sar16(num) (br_int_32)(br_int_16)((br_uint_32)(num) >> 16)

extern int BR_ASM_CALL SafeFixedMac2Div(int, int, int, int, int);

typedef void BR_ASM_CALL ScanLineCallFn(void);
typedef ScanLineCallFn *ScanLineCall;
typedef void BR_ASM_CALL TrapezoidRenderCall(void);

TrapezoidRenderCall TrapezoidRenderPIZ2TA;
TrapezoidRenderCall TrapezoidRenderPIZ2TA_RGB_555;
TrapezoidRenderCall TrapezoidRenderPIZ2TA_RGB_888;

TrapezoidRenderCall TrapezoidRenderPIZ2TAN;
TrapezoidRenderCall TrapezoidRenderPIZ2TAN_RGB_555;
TrapezoidRenderCall TrapezoidRenderPIZ2TAN_RGB_888;

TrapezoidRenderCall TrapezoidRenderPIZ2TIA;
TrapezoidRenderCall TrapezoidRenderPIZ2TIA_RGB_555;
TrapezoidRenderCall TrapezoidRenderPIZ2TIA_RGB_888;

static inline void TriangleRender_Generic_AWTMI(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                  struct temp_vertex_fixed *c, br_boolean use_z_buffer, br_uint_32 bpp,
                                  TrapezoidRenderCall trapezoid_render, br_boolean use_bump,
                                  br_boolean use_light)
{
    /*
     * v[0] and v[1] contain the (fractional) screen coordinates
     * And comp[C_W] contain the 3-space z-value
     *
     * (v[0]-width/2)*comp[C_W] gives the 3-space x-values
     * Similarly for y
     */

    int dv_int;
    int offset;
    int grad;

    br_fixed_ls g_divisor, dp1, dp2;

    int dv_int_nocarry;
    int dv_int_carry;
    int v_int_current;
    int direction;

    int texture_height;
    int colour_width;

    texture_height = zb.material->colour_map->height;
    zb.awsl.texture_stride = zb.material->colour_map->row_bytes;
    zb.awsl.texture_width = zb.material->colour_map->width;

    /* Optimal sort of 3 elements? */
    if (a->v[1] > b->v[1])
    {
        if (b->v[1] > c->v[1])
        {
            swap(struct temp_vertex_fixed *, a, c);
        }
        else if (a->v[1] > c->v[1])
        {
            swap(struct temp_vertex_fixed *, a, b);
            swap(struct temp_vertex_fixed *, b, c);
        }
        else
        {
            swap(struct temp_vertex_fixed *, a, b);
        }
    }
    else if (b->v[1] > c->v[1])
    {
        if (a->v[1] > c->v[1])
        {
            swap(struct temp_vertex_fixed *, b, c);
            swap(struct temp_vertex_fixed *, a, b);
        }
        else
        {
            swap(struct temp_vertex_fixed *, b, c);
        }
    }

    screen_scalar sxa, sxb, sxc, sya, syb, syc;
    sxa = BrFixedToInt(a->v[0]);
    sya = BrFixedToInt(a->v[1]);
    sxb = BrFixedToInt(b->v[0]);
    syb = BrFixedToInt(b->v[1]);
    sxc = BrFixedToInt(c->v[0]);
    syc = BrFixedToInt(c->v[1]);

    colour_width = zb.row_width / bpp;

    zb.top.x = sxb - sxa;
    zb.top.y = syb - sya;
    zb.top.count = syb - sya;
    if (zb.top.count != 0)
    {
        zb.top.grad = BrFixedDiv(zb.top.x, zb.top.y);
        zb.top.d_f = (zb.top.grad << 16);
        zb.top.d_i = (zb.top.grad >> 16) + colour_width;
    }

    zb.bot.x = sxc - sxb;
    zb.bot.y = syc - syb;
    zb.bot.count = syc - syb;
    if (zb.bot.count != 0)
    {
        zb.bot.grad = BrFixedDiv(zb.bot.x, zb.bot.y);
        zb.bot.d_f = (zb.bot.grad << 16);
        zb.bot.d_i = (zb.bot.grad >> 16) + colour_width;
    }

    zb.main.x = sxc - sxa;
    zb.main.y = syc - sya;
    if (zb.main.y != 0)
    {
        zb.main.grad = BrFixedDiv(zb.main.x, zb.main.y);
    }
    else
    {
        zb.main.grad = 0;
    }
    zb.main.d_f = (zb.main.grad << 16);
    zb.main.d_i = (zb.main.grad >> 16) + colour_width;

    zb.main.f = (1 << 31);
    zb.main.i = 0;
    zb.top.f = (1 << 31);
    zb.top.i = 0;
    zb.bot.f = (1 << 31);
    zb.bot.i = 0;

    zb.awsl.texture_stride = zb.material->colour_map->row_bytes;
    zb.awsl.texture_width = zb.material->colour_map->width;
    texture_height = zb.material->colour_map->height;
    zb.awsl.texture_size = zb.awsl.texture_stride * texture_height;
    zb.awsl.texture_start = zb.texture_buffer;
    offset = BrFixedToInt(a->v[X]) + BrFixedToInt(a->v[Y]) * colour_width;
    zb.awsl.start = zb.awsl.end = zb.colour_buffer + offset * bpp;
    zb.awsl.zstart = (br_fixed_ls *)((br_uint_16 *)zb.depth_buffer + offset);

    g_divisor = zb.top.x * zb.main.y - zb.main.x * zb.top.y;
    if (g_divisor == 0)
    {
        return;
    }

    direction = zb.top.count && zb.top.grad >= zb.main.grad || zb.bot.count && zb.bot.grad < zb.main.grad;

#define PARAM_SETUP(param, p)                                                                                          \
    {                                                                                                                  \
        dp1 = b->p - a->p;                                                                                             \
        dp2 = c->p - a->p;                                                                                             \
        param.grad_x = SafeFixedMac2Div(dp1, zb.main.y, dp2, -zb.top.y, g_divisor);                                    \
        param.grad_y = SafeFixedMac2Div(dp2, zb.top.x, dp1, -zb.main.x, g_divisor);                                    \
        param.current = a->p + param.grad_x / 2;                                                                       \
        param.d_nocarry = BrFixedToInt(zb.main.grad) * param.grad_x + param.grad_y;                                    \
        param.d_carry = param.d_nocarry + param.grad_x;                                                                \
    }

    if (use_light)
    {
        PARAM_SETUP(zb.pi, comp[C_I]);
    }

    if (use_z_buffer)
    {
        PARAM_SETUP(zb.pz, v[Z]);
    }

    PARAM_SETUP(zb.pu, comp[C_U]);
    PARAM_SETUP(zb.pv, comp[C_V]);

    zb.awsl.du_nocarry = zb.pu.d_nocarry << 16;
    zb.awsl.dv_nocarry = zb.pv.d_nocarry << 16;
    zb.awsl.du_carry = zb.pu.d_carry << 16;
    zb.awsl.dv_carry = zb.pv.d_carry << 16;
    zb.awsl.u_current = zb.pu.current << 16;
    zb.awsl.v_current = zb.pv.current << 16;

    /*
     * Per scan line increments
     */
    if (__BrBitHaxIsPow2(zb.awsl.texture_width))
    {
        zb.awsl.du_int_nocarry = (sar16(zb.pu.d_nocarry) & zb.awsl.texture_width - 1) - zb.awsl.texture_width;
        zb.awsl.du_int_carry = (sar16(zb.pu.d_carry) & zb.awsl.texture_width - 1) - zb.awsl.texture_width;
        zb.awsl.u_int_current = (sar16(zb.pu.current) & zb.awsl.texture_width - 1);
    }
    else
    {
        zb.awsl.du_int_nocarry = sar16(zb.pu.d_nocarry) % zb.awsl.texture_width;
        zb.awsl.du_int_carry = sar16(zb.pu.d_carry) % zb.awsl.texture_width;
        zb.awsl.u_int_current = sar16(zb.pu.current) % zb.awsl.texture_width;
        if (zb.awsl.du_int_nocarry >= 0)
        {
            zb.awsl.du_int_nocarry -= zb.awsl.texture_width;
        }
        if (zb.awsl.du_int_carry >= 0)
        {
            zb.awsl.du_int_carry -= zb.awsl.texture_width;
        }
        if (zb.awsl.u_int_current < 0)
        {
            zb.awsl.u_int_current += zb.awsl.texture_width;
        }
    }

    if (__BrBitHaxIsPow2(texture_height))
    {
        dv_int_nocarry = (sar16(zb.pv.d_nocarry) & texture_height - 1) - texture_height;
        dv_int_carry = (sar16(zb.pv.d_carry) & texture_height - 1) - texture_height;
        v_int_current = (sar16(zb.pv.current) & texture_height - 1);
    }
    else
    {
        dv_int_nocarry = sar16(zb.pv.d_nocarry) % texture_height;
        dv_int_carry = sar16(zb.pv.d_carry) % texture_height;
        v_int_current = sar16(zb.pv.current) % texture_height;
        if (dv_int_nocarry >= 0)
        {
            dv_int_nocarry -= texture_height;
        }
        if (dv_int_carry >= 0)
        {
            dv_int_carry -= texture_height;
        }
        if (v_int_current < 0)
        {
            v_int_current += texture_height;
        }
    }

    zb.awsl.dsource_nocarry = zb.awsl.du_int_nocarry * bpp + zb.awsl.texture_stride * dv_int_nocarry;
    zb.awsl.dsource_carry = zb.awsl.du_int_carry * bpp + zb.awsl.texture_stride * dv_int_carry;
    zb.awsl.source_current = zb.texture_buffer + zb.awsl.u_int_current * bpp + zb.awsl.texture_stride * v_int_current;
    zb.awsl.u_int_current += 0x8000;

    if (direction)
    {
        zb.awsl.du = zb.pu.grad_x << 16;
        zb.awsl.dv = zb.pv.grad_x << 16;
        zb.awsl.du_int = sar16(zb.pu.grad_x) % zb.awsl.texture_width;
        if (__BrBitHaxIsPow2(texture_height))
        {
            dv_int = (sar16(zb.pv.grad_x) & texture_height - 1) - texture_height;
        }
        else
        {
            dv_int = sar16(zb.pv.grad_x) % texture_height;
            if (dv_int >= 0)
            {
                dv_int -= texture_height;
            }
        }
    }
    else
    {
        zb.awsl.du = -zb.pu.grad_x << 16;
        zb.awsl.dv = -zb.pv.grad_x << 16;
        zb.awsl.du_int = sar16(-zb.pu.grad_x) % zb.awsl.texture_width;
        if (__BrBitHaxIsPow2(texture_height))
        {
            dv_int = (sar16(-zb.pv.grad_x) & texture_height - 1) - texture_height;
        }
        else
        {
            dv_int = (sar16(-zb.pv.grad_x) % texture_height);
        }
        if (dv_int >= 0)
        {
            dv_int -= texture_height;
        }
    }

    if (zb.awsl.du_int >= 0)
    {
        zb.awsl.du_int -= zb.awsl.texture_width;
    }

    zb.awsl.dsource = zb.awsl.du_int * bpp + zb.awsl.texture_stride * dv_int;
    zb.awsl.edge = &zb.top;

    ASSERT(zb.awsl.source_current >= zb.awsl.texture_start);

    trapezoid_render();

    zb.awsl.end = zb.colour_buffer + high16(b->v[X]) * bpp + high16(b->v[Y]) * zb.row_width;
    zb.awsl.edge = &zb.bot;

    ASSERT(zb.awsl.source_current >= zb.awsl.texture_start);
    trapezoid_render();
}

void BR_ASM_CALL TriangleRenderPIZ2TIA(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                       struct temp_vertex_fixed *c)
{
    TriangleRender_Generic_AWTMI(a, b, c, 1, 1, TrapezoidRenderPIZ2TIA, 0, 1);
}

void BR_ASM_CALL TriangleRenderPIZ2TA(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                      struct temp_vertex_fixed *c)
{
    TriangleRender_Generic_AWTMI(a, b, c, 1, 1, TrapezoidRenderPIZ2TA, 0, 0);
}

void BR_ASM_CALL TriangleRenderPIZ2TAN(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                       struct temp_vertex_fixed *c)
{
    TriangleRender_Generic_AWTMI(a, b, c, 1, 1, TrapezoidRenderPIZ2TAN, 1, 0);
}

void BR_ASM_CALL TriangleRenderPIZ2TIA_RGB_555(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                               struct temp_vertex_fixed *c)
{
    TriangleRender_Generic_AWTMI(a, b, c, 1, 2, TrapezoidRenderPIZ2TIA_RGB_555, 0, 1);
}

void BR_ASM_CALL TriangleRenderPIZ2TA_RGB_555(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                              struct temp_vertex_fixed *c)
{
    TriangleRender_Generic_AWTMI(a, b, c, 1, 2, TrapezoidRenderPIZ2TA_RGB_555, 0, 0);
}

void BR_ASM_CALL TriangleRenderPIZ2TAN_RGB_555(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                               struct temp_vertex_fixed *c)
{
    TriangleRender_Generic_AWTMI(a, b, c, 1, 2, TrapezoidRenderPIZ2TAN_RGB_555, 1, 0);
}

void BR_ASM_CALL TriangleRenderPIZ2TIA_RGB_888(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                               struct temp_vertex_fixed *c)
{
    TriangleRender_Generic_AWTMI(a, b, c, 1, 3, TrapezoidRenderPIZ2TIA_RGB_888, 0, 1);
}

void BR_ASM_CALL TriangleRenderPIZ2TA_RGB_888(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                              struct temp_vertex_fixed *c)
{
    TriangleRender_Generic_AWTMI(a, b, c, 1, 3, TrapezoidRenderPIZ2TA_RGB_888, 0, 0);
}

void BR_ASM_CALL TriangleRenderPIZ2TAN_RGB_888(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b,
                                               struct temp_vertex_fixed *c)
{
    TriangleRender_Generic_AWTMI(a, b, c, 1, 3, TrapezoidRenderPIZ2TAN_RGB_888, 1, 0);
}
