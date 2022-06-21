/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: innera.inc.c 1.7 1995/08/31 16:47:32 sam Exp $
 *
 * Triangle renderer for arbitrary width linear texture mapping
 */
void BR_ASM_CALL FNAME(struct temp_vertex *_a, struct temp_vertex *_b, struct temp_vertex *_c)
{
    struct temp_vertex_fixed *a = (void *)_a;
    struct temp_vertex_fixed *b = (void *)_b;
    struct temp_vertex_fixed *c = (void *)_c;

    /* v[0] and v[1] contain the (fractional) screen coordinates */
    /* And comp[C_W] contain the 3-space z-value */

    /* (v[0]-width/2)*comp[C_W] gives the 3-space x-values */
    /* Similarly for y */

    float sxa, sxb, sxc, sya, syb, syc;

    int dv_int;
    int offset;
    int grad;

#if FIX
    br_fixed_ls g_divisor, dp1, dp2;
#else
    float g_divisor, dp1, dp2;
#endif

    int dv_int_nocarry;
    int dv_int_carry;
    int v_int_current;
    int direction;

    int texture_height;
    int colour_width;

    // TrapezoidRenderCall *TrapezoidRender;

    texture_height = zb.material->colour_map->height;
    awsl.texture_stride = zb.material->colour_map->row_bytes;
    awsl.texture_width = zb.material->colour_map->width;

    // Optimal sort of 3 elements?
    if (a->v[1] > b->v[1])
    {
        if (b->v[1] > c->v[1])
        {
            swap(struct temp_vertex_fixed *, a, c);
        }
        else
        {
            if (a->v[1] > c->v[1])
            {
                swap(struct temp_vertex_fixed *, a, b);
                swap(struct temp_vertex_fixed *, b, c);
            }
            else
            {
                swap(struct temp_vertex_fixed *, a, b);
            }
        }
    }
    else
    {
        if (b->v[1] > c->v[1])
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
    }

    sxa = sar16(a->v[0]);
    sya = sar16(a->v[1]);
    sxb = sar16(b->v[0]);
    syb = sar16(b->v[1]);
    sxc = sar16(c->v[0]);
    syc = sar16(c->v[1]);

    zb.top.x = sxb - sxa;
    zb.top.y = syb - sya;
    zb.bot.x = sxc - sxb;
    zb.bot.y = syc - syb;
    zb.main.x = sxc - sxa;
    zb.main.y = syc - sya;

    awsl.texture_stride = zb.material->colour_map->row_bytes;
    awsl.texture_width = zb.material->colour_map->width;
    texture_height = zb.material->colour_map->height;
    awsl.texture_size = awsl.texture_stride * texture_height;
    awsl.texture_start = zb.texture_buffer;

    colour_width = zb.row_width / BPP;

    zb.main.grad = zb.main.x * _reciprocal[zb.main.y];
    zb.main.d_f = zb.main.grad << 16;
    grad = sar16(zb.main.grad);
    zb.main.d_i = grad + colour_width;

    zb.top.grad = zb.top.y ? zb.top.x * _reciprocal[zb.top.y] : 0;
    zb.top.d_f = zb.top.grad << 16;
    zb.top.d_i = sar16(zb.top.grad) + colour_width;

    zb.main.f = zb.top.f = 0x80000000;

    offset = high16(a->v[X]) + high16(a->v[Y]) * colour_width;
    awsl.start = awsl.end = zb.colour_buffer + offset * BPP;
    awsl.zstart = (char *)zb.depth_buffer + offset * 2;
    zb.top.count = syb - sya;

    zb.bot.f = 0x80000000;

    zb.bot.grad = zb.bot.y ? zb.bot.x * _reciprocal[zb.bot.y] : 0;
    zb.bot.d_f = zb.bot.grad << 16;
    zb.bot.d_i = sar16(zb.bot.grad) + colour_width;
    zb.bot.count = syc - syb;

    g_divisor = zb.top.x * zb.main.y - zb.main.x * zb.top.y;
    if (g_divisor == 0)
    {
        return;
    }

    direction = zb.top.count != 0 && zb.top.grad >= zb.main.grad || zb.bot.count != 0 && zb.bot.grad < zb.main.grad;

#if FIX
#define PARAM_SETUP(param, p)                                                                                          \
    {                                                                                                                  \
        dp1 = b->p - a->p;                                                                                             \
        dp2 = c->p - a->p;                                                                                             \
        param.grad_x = SafeFixedMac2Div(dp1, zb.main.y, -dp2, zb.top.y, g_divisor);                                    \
        param.grad_y = SafeFixedMac2Div(dp2, zb.top.x, -dp1, zb.main.x, g_divisor);                                    \
        param.current = a->p + param.grad_x / 2;                                                                       \
        param.d_nocarry = BrFixedToInt(zb.main.grad) * param.grad_x + param.grad_y;                                    \
        param.d_carry = param.d_nocarry + param.grad_x;                                                                \
    }
#else
#define PARAM_SETUP(param, p)                                                                                          \
    {                                                                                                                  \
        dp1 = b->p - a->p;                                                                                             \
        dp2 = c->p - a->p;                                                                                             \
        param.grad_x = BrFloatToFixed((dp1 * BrIntToFixed(zb.main.y) - dp2 * BrIntToFixed(zb.top.y)) / g_divisor);     \
        param.grad_y = BrFloatToFixed((dp2 * BrIntToFixed(zb.top.x) - dp1 * BrIntToFixed(zb.main.x)) / g_divisor);     \
        param.current = a->p + param.grad_x / 2;                                                                       \
        param.d_nocarry = BrFixedToInt(zb.main.grad) * param.grad_x + param.grad_y;                                    \
        param.d_carry = param.d_nocarry + param.grad_x;                                                                \
    }
#endif

#if LIGHT
    PARAM_SETUP(zb.pi, comp[C_I]);
#endif
    PARAM_SETUP(zb.pz, v[Z]);

    PARAM_SETUP(zb.pu, comp[C_U]);
    PARAM_SETUP(zb.pv, comp[C_V]);

    awsl.du_nocarry = zb.pu.d_nocarry << 16;
    awsl.dv_nocarry = zb.pv.d_nocarry << 16;
    awsl.du_carry = zb.pu.d_carry << 16;
    awsl.dv_carry = zb.pv.d_carry << 16;
    awsl.u_current = zb.pu.current << 16;
    awsl.v_current = zb.pv.current << 16;

    /* Per scan line increments */

#if WRAP
    if (powerof2(awsl.texture_width))
    {
        awsl.du_int_nocarry = (sar16(zb.pu.d_nocarry) & awsl.texture_width - 1) - awsl.texture_width;
        awsl.du_int_carry = (sar16(zb.pu.d_carry) & awsl.texture_width - 1) - awsl.texture_width;
        awsl.u_int_current = sar16(zb.pu.current) & awsl.texture_width - 1;
    }
    else
    {
        awsl.du_int_nocarry = sar16(zb.pu.d_nocarry) % awsl.texture_width;
        awsl.du_int_carry = sar16(zb.pu.d_carry) % awsl.texture_width;
        awsl.u_int_current = sar16(zb.pu.current) % awsl.texture_width;
        if (awsl.du_int_nocarry >= 0)
            awsl.du_int_nocarry -= awsl.texture_width;
        if (awsl.du_int_carry >= 0)
            awsl.du_int_carry -= awsl.texture_width;
        if (awsl.u_int_current < 0)
            awsl.u_int_current += awsl.texture_width;
    }

    if (powerof2(texture_height))
    {
        dv_int_nocarry = (sar16(zb.pv.d_nocarry) & texture_height - 1) - texture_height;
        dv_int_carry = (sar16(zb.pv.d_carry) & texture_height - 1) - texture_height;
        v_int_current = sar16(zb.pv.current) & texture_height - 1;
    }
    else
    {
        dv_int_nocarry = sar16(zb.pv.d_nocarry) % texture_height;
        dv_int_carry = sar16(zb.pv.d_carry) % texture_height;
        v_int_current = sar16(zb.pv.current) % texture_height;
        if (dv_int_nocarry >= 0)
            dv_int_nocarry -= texture_height;
        if (dv_int_carry >= 0)
            dv_int_carry -= texture_height;
        if (v_int_current < 0)
            v_int_current += texture_height;
    }

    awsl.dsource_nocarry = awsl.du_int_nocarry * BPP + awsl.texture_stride * dv_int_nocarry;
    awsl.dsource_carry = awsl.du_int_carry * BPP + awsl.texture_stride * dv_int_carry;
    awsl.source_current = zb.texture_buffer + awsl.u_int_current * BPP + awsl.texture_stride * v_int_current;
    awsl.u_int_current += 0x8000;

    if (direction)
    {
        awsl.du = zb.pu.grad_x << 16;
        awsl.dv = zb.pv.grad_x << 16;
        awsl.du_int = sar16(zb.pu.grad_x) % awsl.texture_width;
        if (powerof2(texture_height))
            dv_int = (sar16(zb.pv.grad_x) & texture_height - 1) - texture_height;
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
        awsl.du = -zb.pu.grad_x << 16;
        awsl.dv = -zb.pv.grad_x << 16;
        awsl.du_int = sar16(-zb.pu.grad_x) % awsl.texture_width;

        if (powerof2(texture_height))
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

    if (awsl.du_int >= 0)
    {
        awsl.du_int -= awsl.texture_width;
    }
#else /* WRAP */

    if (powerof2(awsl.texture_width))
    {
        awsl.du_int_nocarry = mod2n(sar16(zb.pu.d_nocarry), awsl.texture_width);
        awsl.du_int_carry = mod2n(sar16(zb.pu.d_carry), awsl.texture_width);
        awsl.u_int_current = sar16(zb.pu.current) & awsl.texture_width - 1;
    }
    else
    {
        awsl.du_int_nocarry = sar16(zb.pu.d_nocarry) % awsl.texture_width;
        awsl.du_int_carry = sar16(zb.pu.d_carry) % awsl.texture_width;
        awsl.u_int_current = sar16(zb.pu.current) % awsl.texture_width;
    }

    if (powerof2(texture_height))
    {
        dv_int_nocarry = mod2n(sar16(zb.pv.d_nocarry), texture_height);
        dv_int_carry = mod2n(sar16(zb.pv.d_carry), texture_height);
        v_int_current = sar16(zb.pv.current) & texture_height - 1;
    }
    else
    {
        dv_int_nocarry = sar16(zb.pv.d_nocarry) % texture_height;
        dv_int_carry = sar16(zb.pv.d_carry) % texture_height;
        v_int_current = sar16(zb.pv.current) % texture_height;
    }

    awsl.dsource_nocarry = awsl.du_int_nocarry * BPP + awsl.texture_stride * dv_int_nocarry;
    awsl.dsource_carry = awsl.du_int_carry * BPP + awsl.texture_stride * dv_int_carry;
    awsl.source_current = zb.texture_buffer + awsl.u_int_current * BPP + awsl.texture_stride * v_int_current;
    awsl.u_int_current += 0x8000;

    if (direction)
    {
        awsl.du = zb.pu.grad_x << 16;
        awsl.dv = zb.pv.grad_x << 16;
        awsl.du_int = sar16(zb.pu.grad_x) % awsl.texture_width;
        if (texture_height & texture_height - 1)
        {
            dv_int = sar16(zb.pv.grad_x) % texture_height;
        }
        else
        {
            dv_int = mod2n(sar16(zb.pv.grad_x), texture_height);
        }
    }
    else
    {
        awsl.du = -zb.pu.grad_x << 16;
        awsl.dv = -zb.pv.grad_x << 16;
        awsl.du_int = sar16(-zb.pu.grad_x) % awsl.texture_width;
        if (texture_height & texture_height - 1)
        {
            dv_int = sar16(-zb.pv.grad_x) % texture_height;
        }
        else
        {
            dv_int = mod2n(sar16(-zb.pv.grad_x), texture_height);
        }
    }

#endif /* WRAP */

    awsl.dsource = awsl.du_int * BPP + awsl.texture_stride * dv_int;
    awsl.edge = &zb.top;

    ASSERT(awsl.source_current >= awsl.texture_start);

#if BPP == 1
#if LIGHT
    TrapezoidRenderPIZ2TIA();
#else
    TrapezoidRenderPIZ2TA();
#endif
#elif BPP == 2
    TrapezoidRenderPIZ2TA_RGB555();
#elif BPP == 3
    TrapezoidRenderPIZ2TA_RGB888();
#endif

    awsl.end = zb.colour_buffer + high16(b->v[X]) * BPP + high16(b->v[Y]) * zb.row_width;
    awsl.edge = &zb.bot;

    ASSERT(awsl.source_current >= awsl.texture_start);

#if BPP == 1
#if LIGHT
    TrapezoidRenderPIZ2TIA();
#else
    TrapezoidRenderPIZ2TA();
#endif
#elif BPP == 2
    TrapezoidRenderPIZ2TA_RGB555();
#elif BPP == 3
    TrapezoidRenderPIZ2TA_RGB888();
#endif
}
