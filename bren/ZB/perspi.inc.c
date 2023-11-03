/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: perspi.inc.c 1.1 1995/08/31 16:52:49 sam Exp $
 *
 * Perspective texture mapped triangle renderer
 */

#define SHIFT 1

/*
 * Macros for moving left, right, up and down in texture whose size is
 * SIZE*SIZE.
 */

#if SIZE == 256 && defined __WATCOMC__

#define incv(a) inch(a)
#define incu(a) incl(a)
#define decv(a) dech(a)
#define decu(a) decl(a)

#else

#define incv(a) (a + SIZE & SIZE * SIZE - 1);
#define incu(a) ((a + 1 & SIZE - 1) | (a & ~(SIZE - 1)));
#define decv(a) (a - SIZE & SIZE * SIZE - 1);
#define decu(a) ((a - 1 & SIZE - 1) | (a & ~(SIZE - 1)));

#endif

/*
 * Setup and outer loops for perspective texture mappers
 */

void BR_ASM_CALL FNAME(struct temp_vertex_fixed *a, struct temp_vertex_fixed *b, struct temp_vertex_fixed *c)
{
    /*
     * In linear texture mapping we interpolate u and v which we then use
     * as the texture coordinates.
     *
     * In perspective texture mapping we linearly interpolate uq, wq and q
     * and use uq/q and wq/q as texture coordinates.
     *
     * q is simply a constant times comp[C_W] chose to scale things for
     * maximal accuracy
     *
     * v[0] and v[1] contain the (fractional) screen coordinates
     */

    /*
     * Screen coordinates
     */
    int sxa, sxb, sxc, sya, syb, syc;

    /*
     * Values of uq,vq and q at vertices
     */
    float au, av, bu, bv, cu, cv;

    br_fixed_ls dp1, dp2, g_divisor;

    int du_numerator_nocarry;
    int dv_numerator_nocarry;
    int u, v;
    int offset;
    int u_base, v_base;

    int carry, grad;

    float aw, bw, cw, maxuv, d1, d2, g_inverse, zmx, zmy, ztx, zty;

    /* Optimal sort of 3 elements? */

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

#define RANGE(min, max, a, b, c)                                                                                       \
    {                                                                                                                  \
        int middle;                                                                                                    \
        min = a;                                                                                                       \
        middle = b;                                                                                                    \
        max = c;                                                                                                       \
        if (min > middle)                                                                                              \
            swap(int, min, middle);                                                                                    \
        if (middle > max)                                                                                              \
            swap(int, middle, max);                                                                                    \
        if (min > middle)                                                                                              \
            min = middle;                                                                                              \
    }

#if CHEAT
    {
        br_fixed_ls min, max, range;

        RANGE(min, max, a->v[0], b->v[0], c->v[0]);

        range = max - min;

        if (c->v[1] - a->v[1] > range)
            range = c->v[1] - a->v[1];

        range /= 0x10000;

        RANGE(min, max, a->comp[C_W], b->comp[C_W], c->comp[C_W]);

        while ((min | max) & 0xffff0000)
        {
            min /= 4;
            max /= 4;
        }

        if ((max - min) * range < cutoff * min)
        {
            LINEAR(a, b, c);
            return;
        }
    }
#endif

    /*
     * Set up general triangle parameters
     */
    sxa = sar16(a->v[0]);
    sxb = sar16(b->v[0]);
    sxc = sar16(c->v[0]);
    sya = sar16(a->v[1]);
    syb = sar16(b->v[1]);
    syc = sar16(c->v[1]);

    if (sya == syc)
        return;

    zb.top.x = sxb - sxa;
    zb.top.y = syb - sya;
    zb.bot.x = sxc - sxb;
    zb.bot.y = syc - syb;
    zb.main.x = sxc - sxa;
    zb.main.y = syc - sya;

    g_divisor = zb.top.x * zb.main.y - zb.main.x * zb.top.y;
    if (g_divisor == 0)
    {
        return;
    }

    if (g_divisor > 0 && g_divisor < 100)
        g_inverse = frcp[g_divisor];
    else if (g_divisor < 0 && g_divisor > -100)
        g_inverse = -frcp[-g_divisor];
    else
        g_inverse = 1.0f / g_divisor;

    /*
     * Set up texture specific parameters
     */

    /*
     * Eliminate rounding errors by shifting to new base point
     */
    u_base = a->comp[C_U] & 0xffff0000;
    v_base = a->comp[C_V] & 0xffff0000;

    u = (a->comp[C_U] - u_base) / 0x10000;
    v = (a->comp[C_V] - v_base) / 0x10000;

    au = (float)(a->comp[C_U] - u_base) / 0x10000;
    bu = (float)(b->comp[C_U] - u_base) / 0x10000;
    cu = (float)(c->comp[C_U] - u_base) / 0x10000;
    av = (float)(a->comp[C_V] - v_base) / 0x10000;
    bv = (float)(b->comp[C_V] - v_base) / 0x10000;
    cv = (float)(c->comp[C_V] - v_base) / 0x10000;

    u_base /= 0x10000;
    v_base /= 0x10000;

    /*
     * Convert texture coordinates u,v to projective coordinates u,v,1/w
     */
    aw = (float)b->comp[C_W] * c->comp[C_W];
    bw = (float)c->comp[C_W] * a->comp[C_W];
    cw = (float)b->comp[C_W] * a->comp[C_W];

    /*
     * Normalise u,v,q to maximise accuracy
     */

    maxuv = aw > 0 ? aw : -aw;
    maxuv += au > 0 ? au : -au;
    maxuv += av > 0 ? av : -av;

    au *= aw;
    maxuv += au > 0 ? au : -au;
    av *= aw;
    maxuv += av > 0 ? av : -av;
    bu *= bw;
    maxuv += bu > 0 ? bu : -bu;
    bv *= bw;
    maxuv += bv > 0 ? bv : -bv;
    cu *= cw;
    maxuv += cu > 0 ? cu : -cu;
    cv *= cw;
    maxuv += cv > 0 ? cv : -cv;

    maxuv = (1 << 28) / maxuv;

    au *= maxuv;
    av *= maxuv;
    aw *= maxuv;
    bu *= maxuv;
    bv *= maxuv;
    bw *= maxuv;
    cu *= maxuv;
    cv *= maxuv;
    cw *= maxuv;

    d1 = bu - au;
    d2 = cu - au;

    /*
     * Convert deltas along edges to deltas along x and y
     */
    zmx = zb.main.x * g_inverse;
    zmy = zb.main.y * g_inverse;
    ztx = zb.top.x * g_inverse;
    zty = zb.top.y * g_inverse;

    zb.pu.grad_x = (br_int_32)(d1 * zmy - d2 * zty);
    zb.pu.grad_y = (br_int_32)(d2 * ztx - d1 * zmx);

    d1 = bv - av;
    d2 = cv - av;

    zb.pv.grad_x = (br_int_32)(d1 * zmy - d2 * zty);
    zb.pv.grad_y = (br_int_32)(d2 * ztx - d1 * zmx);

    d1 = bw - aw;
    d2 = cw - aw;

    zb.pq.grad_x = (br_int_32)(d1 * zmy - d2 * zty);
    zb.pq.grad_y = (br_int_32)(d2 * ztx - d1 * zmx);

    zb.pu.current = (br_int_32)au;
    zb.pv.current = (br_int_32)av;
    zb.pq.current = (br_int_32)aw;

    if (!zb.pq.current)
    {
        return;
    }

    /*
     * Calculate deltas along triangle edges
     */

    zb.main.grad = zb.main.x * _reciprocal[zb.main.y];
    zb.main.d_f = zb.main.grad << 16;
    grad = sar16(zb.main.grad);
    zb.main.d_i = grad + zb.row_width;

    zb.pu.d_nocarry = grad * zb.pu.grad_x + zb.pu.grad_y;
    zb.pv.d_nocarry = grad * zb.pv.grad_x + zb.pv.grad_y;
    zb.pq.d_nocarry = grad * zb.pq.grad_x + zb.pq.grad_y;

    zb.top.grad = zb.top.y ? zb.top.x * _reciprocal[zb.top.y] : 0;
    zb.top.d_f = zb.top.grad << 16;
    zb.top.d_i = sar16(zb.top.grad) + zb.row_width;
    zb.top.count = syb - sya;

    zb.bot.grad = zb.bot.y ? zb.bot.x * _reciprocal[zb.bot.y] : 0;
    zb.bot.d_f = zb.bot.grad << 16;
    zb.bot.d_i = sar16(zb.bot.grad) + zb.row_width;
    zb.bot.count = syc - syb;

#define PARAM_SETUP(param, p)                                                                                          \
    {                                                                                                                  \
        dp1 = b->p - a->p;                                                                                             \
        dp2 = c->p - a->p;                                                                                             \
        param.grad_x = SafeFixedMac2Div(dp1, zb.main.y, -dp2, zb.top.y, g_divisor);                                    \
        param.grad_y = SafeFixedMac2Div(dp2, zb.top.x, -dp1, zb.main.x, g_divisor);                                    \
        param.current = a->p + (g_divisor >= 0 ? param.grad_x / 2 : -param.grad_x / 2);                                \
        param.d_nocarry = BrFixedToInt(zb.main.grad) * param.grad_x + param.grad_y;                                    \
        param.d_carry = param.d_nocarry + param.grad_x;                                                                \
    }

#if LIGHT
    PARAM_SETUP(zb.pi, comp[C_I]);
#endif

    /* Shift right by half a pixel */

    zb.main.f = zb.top.f = zb.bot.f = 0x80000000;

    du_numerator_nocarry = u * zb.pq.d_nocarry - zb.pu.d_nocarry;
    zb.pu.grad_x = u * zb.pq.grad_x - zb.pu.grad_x;

    dv_numerator_nocarry = v * zb.pq.d_nocarry - zb.pv.d_nocarry;
    zb.pv.grad_x = v * zb.pq.grad_x - zb.pv.grad_x;

    /* u_numerator and v_numerator are the 'Bresenham style' error terms */

    zb.pu.current = zb.pu.current - u * zb.pq.current;
    zb.pv.current = zb.pv.current - v * zb.pq.current;

    /*
     * Set up pointer into texture taking into account the fact that
     * its size may be an arbitrary power of two
     */

    zb.tsl.source = (v + v_base & SIZE - 1) * SIZE | u + u_base & SIZE - 1;

    offset = sar16(a->v[X]) + sar16(a->v[Y]) * zb.row_width - (g_divisor < 0);
    zb.tsl.start = zb.tsl.end = zb.colour_buffer + offset;
    zb.tsl.zstart = (char *)zb.depth_buffer + (offset << 1);

    PARAM_SETUP(zb.pz, v[Z]);

#if SHIFT

    /* shift by 1/2 pixel */

    if (g_divisor >= 0)
    {
        zb.pq.current += zb.pq.grad_x / 2;
        zb.pu.current += -zb.pu.grad_x / 2;
        zb.pv.current += -zb.pv.grad_x / 2;
    }
    else
    {
        zb.pq.current -= zb.pq.grad_x / 2;
        zb.pu.current -= -zb.pu.grad_x / 2;
        zb.pv.current -= -zb.pv.grad_x / 2;
    }

#define PDIVIDE                                                                                                        \
    while (zb.pu.current >= (signed)zb.pq.current)                                                                     \
    {                                                                                                                  \
        zb.pu.current -= zb.pq.current;                                                                                \
        zb.tsl.source = incu(zb.tsl.source);                                                                           \
        zb.pu.grad_x += zb.pq.grad_x;                                                                                  \
        du_numerator_nocarry += zb.pq.d_nocarry;                                                                       \
    }                                                                                                                  \
    while (zb.pu.current < 0)                                                                                          \
    {                                                                                                                  \
        zb.pu.current += zb.pq.current;                                                                                \
        zb.tsl.source = decu(zb.tsl.source);                                                                           \
        zb.pu.grad_x -= zb.pq.grad_x;                                                                                  \
        du_numerator_nocarry -= zb.pq.d_nocarry;                                                                       \
    }                                                                                                                  \
    while (zb.pv.current >= (signed)zb.pq.current)                                                                     \
    {                                                                                                                  \
        zb.pv.current -= zb.pq.current;                                                                                \
        zb.tsl.source = incv(zb.tsl.source);                                                                           \
        zb.pv.grad_x += zb.pq.grad_x;                                                                                  \
        dv_numerator_nocarry += zb.pq.d_nocarry;                                                                       \
    }                                                                                                                  \
    while (zb.pv.current < 0)                                                                                          \
    {                                                                                                                  \
        zb.pv.current += zb.pq.current;                                                                                \
        zb.tsl.source = decv(zb.tsl.source);                                                                           \
        zb.pv.grad_x -= zb.pq.grad_x;                                                                                  \
        dv_numerator_nocarry -= zb.pq.d_nocarry;                                                                       \
    }
#endif

    zb.tsl.y = sya;

    while (zb.top.count--)
    {
        /* Start of second innermost loop */

        /*
         * Call scan line renderer
         */
        if (zb.tsl.start != zb.tsl.end)
        {
            PDIVIDE;
            SNAME();
        }

        zb.tsl.y++;

        zb.top.f += zb.top.d_f;
        carry = (unsigned)zb.top.f < (unsigned)zb.top.d_f;
        zb.tsl.end += zb.top.d_i + carry;
        zb.main.f += zb.main.d_f;
        carry = (unsigned)zb.main.f < (unsigned)zb.main.d_f;
        zb.pq.current += zb.pq.d_nocarry;

        zb.pu.current += -du_numerator_nocarry;
        zb.pv.current += -dv_numerator_nocarry;
        zb.tsl.start += zb.main.d_i + carry;
        zb.tsl.zstart += zb.main.d_i + carry << 1;
#if LIGHT
        zb.pi.current += zb.pi.d_nocarry;
#endif
        zb.pz.current += zb.pz.d_nocarry;
        if (carry)
        {
            zb.pq.current += zb.pq.grad_x;

            zb.pu.current += -zb.pu.grad_x;
            zb.pv.current += -zb.pv.grad_x;
#if LIGHT
            zb.pi.current += zb.pi.grad_x;
#endif
#if ZB
            zb.pz.current += zb.pz.grad_x;
#endif
        }

        /*
         * The core perspective calculations along the main edge
         */

        /* End of second innermost loop */
    }

    zb.tsl.end = zb.colour_buffer + sar16(b->v[X]) + sar16(b->v[Y]) * zb.row_width - (g_divisor < 0);

    while (zb.bot.count--)
    {
        /* Start of second innermost loop */

        /*
         * Call scan line renderer
         */
        if (zb.tsl.start != zb.tsl.end)
        {
            PDIVIDE;
            SNAME();
        }

        zb.tsl.y++;

        zb.bot.f += zb.bot.d_f;
        carry = (unsigned)zb.bot.f < (unsigned)zb.bot.d_f;
        zb.tsl.end += zb.bot.d_i + carry;
        zb.main.f += zb.main.d_f;
        carry = (unsigned)zb.main.f < (unsigned)zb.main.d_f;
        zb.pq.current += zb.pq.d_nocarry;

        zb.pu.current += -du_numerator_nocarry;
        zb.pv.current += -dv_numerator_nocarry;
        zb.tsl.start += zb.main.d_i + carry;
        zb.tsl.zstart += zb.main.d_i + carry << 1;
#if LIGHT
        zb.pi.current += zb.pi.d_nocarry;
#endif
#if ZB
        zb.pz.current += zb.pz.d_nocarry;
#endif
        if (carry)
        {
            zb.pq.current += zb.pq.grad_x;

            zb.pu.current += -zb.pu.grad_x;
            zb.pv.current += -zb.pv.grad_x;
#if LIGHT
            zb.pi.current += zb.pi.grad_x;
#endif
#if ZB
            zb.pz.current += zb.pz.grad_x;
#endif
        }
        /* End of second innermost loop */
    }
}

#undef decu
#undef decv
#undef incu
#undef incv