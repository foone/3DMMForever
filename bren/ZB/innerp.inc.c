/* $Id: innerp.inc.c 1.4 1995/03/01 15:49:34 sam Exp $ */
/* Perspective texture mapped triangle renderer */

#define SHIFT 1

/* Macros for moving left, right, up and down in texture */
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
    float WbWc, WcWa, WaWb;
    float bvcx, bvcy, bvcz, cvax, cvay, cvaz, avbx, avby, avbz;
    float dUx, dUy, dUz, dVx, dVy, dVz, nx, ny;
    float fW;
    float au, av, bu, bv, cu, cv;

    unsigned int iU, iV;
    int idUx, idUy, idVx, idVy, idWx, idWy;

#if FIX
    br_fixed_ls
#else
    float
#endif
        dp1,
        dp2, g_divisor;

    /* We need two sets of iterators: */
    /* One for following scan lines */
    /* Another for following the start of scan lines */

    int udW_nocarry;
    int vdW_nocarry;
    int dU_nocarry, dV_nocarry, dW_nocarry;
    int udom, vdom;
    int offset;
    int u_base, v_base;

    int carry, grad;

    float tmpf;
    int tmpi, tmp1, tmp2, tmp3;
    int range;

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

    if (PerspCheat)
    {

        range = a->v[0] - b->v[0];
        if ((tmpi = a->v[0] - c->v[0]) > range)
            range = tmpi;
        if ((tmpi = b->v[0] - c->v[0]) > range)
            range = tmpi;
        if ((tmpi = b->v[0] - a->v[0]) > range)
            range = tmpi;
        if ((tmpi = c->v[0] - a->v[0]) > range)
            range = tmpi;
        if ((tmpi = c->v[0] - b->v[0]) > range)
            range = tmpi;

        if ((tmpi = c->v[1] - a->v[1]) > range)
            range = tmpi;
        range /= 0x10000;

        tmp1 = a->comp[C_W];
        tmp2 = b->comp[C_W];
        tmp3 = c->comp[C_W];
        if (tmp1 > tmp2)
            swap(int, tmp1, tmp2);
        if (tmp2 > tmp3)
            swap(int, tmp2, tmp3);
        if (tmp1 > tmp2)
            tmp1 = tmp2;
        if (tmp3 < 0)
            return;
        while ((tmp1 | tmp3) & 0xffff0000)
        {
            tmp1 /= 2;
            tmp3 /= 2;
        }

        if ((tmp3 - tmp1) * range < cutoff * tmp1)
        {
#if SIZE == 256 && LIGHT
            TriangleRenderPIZ2TI(_a, _b, _c);
#elif SIZE == 256 && !LIGHT
            TriangleRenderPIZ2T(_a, _b, _c);
#elif LIGHT
            TriangleRenderPIZ2TIA(_a, _b, _c);
#elif !LIGHT
            TriangleRenderPIZ2TA(_a, _b, _c);
#endif
            return;
        }
    }

    sxa = sar16(a->v[0] - fw.vp_width);
    sxb = sar16(b->v[0] - fw.vp_width);
    sxc = sar16(c->v[0] - fw.vp_width);
    sya = sar16(a->v[1] - fw.vp_height);
    syb = sar16(b->v[1] - fw.vp_height);
    syc = sar16(c->v[1] - fw.vp_height);

    if (sya == syc)
        return;

    /* The compiler hopefully has the sense to substitute multiplications here */

    u_base = a->comp[C_U];
    if (b->comp[C_U] < u_base)
        u_base = b->comp[C_U];
    if (c->comp[C_U] < u_base)
        u_base = c->comp[C_U];
    v_base = a->comp[C_V];
    if (b->comp[C_V] < v_base)
        v_base = b->comp[C_V];
    if (c->comp[C_V] < v_base)
        v_base = c->comp[C_V];
    u_base &= 0xffff0000;
    v_base &= 0xffff0000;

    au = (float)(a->comp[C_U] - u_base) / 0x10000;
    bu = (float)(b->comp[C_U] - u_base) / 0x10000;
    cu = (float)(c->comp[C_U] - u_base) / 0x10000;
    av = (float)(a->comp[C_V] - v_base) / 0x10000;
    bv = (float)(b->comp[C_V] - v_base) / 0x10000;
    cv = (float)(c->comp[C_V] - v_base) / 0x10000;

    u_base /= 0x10000;
    v_base /= 0x10000;

    WbWc = (float)b->comp[C_W] * (float)c->comp[C_W];
    WcWa = (float)c->comp[C_W] * (float)a->comp[C_W];
    WaWb = (float)a->comp[C_W] * (float)b->comp[C_W];

    /* Compute cross products a x b using snapped integer coordinates */

    bvcx = WbWc * (syb - syc);
    bvcy = WbWc * (sxc - sxb);
    bvcz = WbWc * (sxb * syc - syb * sxc);

    cvax = WcWa * (syc - sya);
    cvay = WcWa * (sxa - sxc);
    cvaz = WcWa * (sxc * sya - syc * sxa);

    avbx = WaWb * (sya - syb);
    avby = WaWb * (sxb - sxa);
    avbz = WaWb * (sxa * syb - sya * sxb);

    /* The u coordinate for the texel mapped to the screen at x,y */
    /* are given by u = int(U(x,y)/V(x,y)) */

    dUx = bvcx * au + cvax * bu + avbx * cu;
    dUy = bvcy * au + cvay * bu + avby * cu;
    dUz = bvcz * au + cvaz * bu + avbz * cu;

    dVx = bvcx * av + cvax * bv + avbx * cv;
    dVy = bvcy * av + cvay * bv + avby * cv;
    dVz = bvcz * av + cvaz * bv + avbz * cv;

    nx = bvcx + cvax + avbx;
    ny = bvcy + cvay + avby;

    fW = nx * sxa + ny * sya + bvcz + cvaz + avbz;

    if (fW < 0)
    {
        fW = -fW;
        dUx = -dUx, dUy = -dUy;
        dVx = -dVx, dVy = -dVy;
        nx = -nx, ny = -ny;
    }

    /* Now put U,V and various deltas into integers */

    /* As u = int(U/V) we can multiply U and V by any factor we choose. */
    /* We choose the factor to make U and V representible by large */
    /* integers for greater accuracy. */

    tmpf = fabs(au) + fabs(bu) + fabs(cu) + fabs(av) + fabs(bv) + fabs(cv);
    tmpf = (float)(1 << 30) / (tmpf * fW);

    idUx = dUx * tmpf;
    idUy = dUy * tmpf;
    idVx = dVx * tmpf;
    idVy = dVy * tmpf;
    tsl.dWx = idWx = nx * tmpf;
    idWy = ny * tmpf;

    tmpf *= fW;

    iU = au * tmpf;
    iV = av * tmpf;
    tsl.iW = tmpf;
    if (!tsl.iW)
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

    /* Calculate deltas along triangle edges */

    zb.main.grad = zb.main.x * _reciprocal[zb.main.y];
    zb.main.d_f = zb.main.grad << 16;
    grad = sar16(zb.main.grad);
    zb.main.d_i = grad + zb.row_width;

    dU_nocarry = grad * idUx + idUy;
    dV_nocarry = grad * idVx + idVy;
    dW_nocarry = grad * idWx + idWy;

    zb.top.grad = zb.top.y ? zb.top.x * _reciprocal[zb.top.y] : 0;
    zb.top.d_f = zb.top.grad << 16;
    zb.top.d_i = sar16(zb.top.grad) + zb.row_width;

    /* Shift right by half a pixel */

    zb.main.f = zb.top.f = 0x80000000;

    udom = au;
    vdom = av;

    udW_nocarry = udom * dW_nocarry - dU_nocarry;
    tsl.udW = udom * idWx - idUx;

    vdW_nocarry = vdom * dW_nocarry - dV_nocarry;
    tsl.vdW = vdom * idWx - idVx;

    /* Eu and Ev are the 'Bresenham style' error terms */

    tsl.Eu = iU - udom * tsl.iW;
    tsl.Ev = iV - vdom * tsl.iW;

    tsl.source = ((vdom + v_base) & SIZE - 1) * SIZE | (udom + u_base) & SIZE - 1;

    offset = sar16(a->v[X]) + sar16(a->v[Y]) * zb.row_width - (g_divisor < 0);
    tsl.start = tsl.end = zb.colour_buffer + offset;
    tsl.zstart = (char *)zb.depth_buffer + (offset << 1);
    zb.top.count = syb - sya;

    zb.bot.f = 0x80000000;

    zb.bot.grad = zb.bot.y ? zb.bot.x * _reciprocal[zb.bot.y] : 0;
    zb.bot.d_f = zb.bot.grad << 16;
    zb.bot.d_i = sar16(zb.bot.grad) + zb.row_width;
    zb.bot.count = syc - syb;

#if FIX
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
#else
#define PARAM_SETUP(param, p)                                                                                          \
    {                                                                                                                  \
        dp1 = BrFixedToFloat(b->p - a->p);                                                                             \
        dp2 = BrFixedToFloat(c->p - a->p);                                                                             \
        param.grad_x = BrFloatToFixed((dp1 * zb.main.y - dp2 * zb.top.y) / g_divisor);                                 \
        param.grad_y = BrFloatToFixed((dp2 * zb.top.x - dp1 * zb.main.x) / g_divisor);                                 \
        param.current = a->p + param.grad_x / 2;                                                                       \
        param.d_nocarry = BrFixedToInt(zb.main.grad) * param.grad_x + param.grad_y;                                    \
        param.d_carry = param.d_nocarry + param.grad_x;                                                                \
    }
#endif

#if LIGHT
    PARAM_SETUP(zb.pi, comp[C_I]);
#endif
    PARAM_SETUP(zb.pz, v[Z]);

#if SHIFT

    /* shift by 1/2 pixel */

    if (g_divisor >= 0)
    {
        tsl.iW += tsl.dWx / 2;
        tsl.Eu += -tsl.udW / 2;
        tsl.Ev += -tsl.vdW / 2;
    }
    else
    {
        tsl.iW -= tsl.dWx / 2;
        tsl.Eu -= -tsl.udW / 2;
        tsl.Ev -= -tsl.vdW / 2;
    }

    if ((signed)tsl.iW < 0)
        return;
    while (tsl.Eu >= (signed)tsl.iW)
    {
        tsl.Eu -= tsl.iW;
        tsl.source = incu(tsl.source, SIZE);
        tsl.udW += tsl.dWx;
        udW_nocarry += dW_nocarry;
    }
    while (tsl.Eu < 0)
    {
        tsl.Eu += tsl.iW;
        tsl.source = decu(tsl.source, SIZE);
        tsl.udW -= tsl.dWx;
        udW_nocarry -= dW_nocarry;
    }
    while (tsl.Ev >= (signed)tsl.iW)
    {
        tsl.Ev -= tsl.iW;
        tsl.source = incv(tsl.source, SIZE);
        tsl.vdW += tsl.dWx;
        vdW_nocarry += dW_nocarry;
    }
    while (tsl.Ev < 0)
    {
        tsl.Ev += tsl.iW;
        tsl.source = decv(tsl.source, SIZE);
        tsl.vdW -= tsl.dWx;
        vdW_nocarry -= dW_nocarry;
    }
#endif

    while (zb.top.count--)
    {
        /* Start of second innermost loop */

#if SIZE == 256 && LIGHT
        ScanLinePIZ2TIP256();
#elif SIZE == 256 && !LIGHT
        ScanLinePIZ2TP256();
#elif SIZE == 64 && LIGHT
        ScanLinePIZ2TIP64();
#elif SIZE == 64 && !LIGHT
        ScanLinePIZ2TP64();
#elif SIZE == 1024 && !LIGHT
        ScanLinePIZ2TP1024();
#elif SIZE == 1024 && LIGHT
        ScanLinePIZ2TIP1024();
#endif

        zb.top.f += zb.top.d_f;
        carry = (unsigned)zb.top.f < (unsigned)zb.top.d_f;
        tsl.end += zb.top.d_i + carry;
        zb.main.f += zb.main.d_f;
        carry = (unsigned)zb.main.f < (unsigned)zb.main.d_f;
        tsl.iW += dW_nocarry;
        tsl.Eu += -udW_nocarry;
        tsl.Ev += -vdW_nocarry;
        tsl.start += zb.main.d_i + carry;
        tsl.zstart += zb.main.d_i + carry << 1;
#if LIGHT
        zb.pi.current += zb.pi.d_nocarry;
#endif
        zb.pz.current += zb.pz.d_nocarry;
        if (carry)
        {
            tsl.iW += idWx;
            tsl.Eu += -tsl.udW;
            tsl.Ev += -tsl.vdW;
#if LIGHT
            zb.pi.current += zb.pi.grad_x;
#endif
            zb.pz.current += zb.pz.grad_x;
        }

        /* The core perspective calculations along the main edge */

        if ((signed)tsl.iW < 0)
            return;
        while (tsl.Eu >= (signed)tsl.iW)
        {
            tsl.Eu -= tsl.iW;
            tsl.source = incu(tsl.source, SIZE);
            tsl.udW += tsl.dWx;
            udW_nocarry += dW_nocarry;
        }
        while (tsl.Eu < 0)
        {
            tsl.Eu += tsl.iW;
            tsl.source = decu(tsl.source, SIZE);
            tsl.udW -= tsl.dWx;
            udW_nocarry -= dW_nocarry;
        }
        while (tsl.Ev >= (signed)tsl.iW)
        {
            tsl.Ev -= tsl.iW;
            tsl.source = incv(tsl.source, SIZE);
            tsl.vdW += tsl.dWx;
            vdW_nocarry += dW_nocarry;
        }
        while (tsl.Ev < 0)
        {
            tsl.Ev += tsl.iW;
            tsl.source = decv(tsl.source, SIZE);
            tsl.vdW -= tsl.dWx;
            vdW_nocarry -= dW_nocarry;
        }
        /* End of second innermost loop */
    }

    tsl.end = zb.colour_buffer + sar16(b->v[X]) + sar16(b->v[Y]) * zb.row_width - (g_divisor < 0);

    while (zb.bot.count--)
    {
        /* Start of second innermost loop */

#if SIZE == 256 && LIGHT
        ScanLinePIZ2TIP256();
#elif SIZE == 256 && !LIGHT
        ScanLinePIZ2TP256();
#elif SIZE == 64 && LIGHT
        ScanLinePIZ2TIP64();
#elif SIZE == 64 && !LIGHT
        ScanLinePIZ2TP64();
#elif SIZE == 1024 && !LIGHT
        ScanLinePIZ2TP1024();
#elif SIZE == 1024 && LIGHT
        ScanLinePIZ2TIP1024();
#endif

        zb.bot.f += zb.bot.d_f;
        carry = (unsigned)zb.bot.f < (unsigned)zb.bot.d_f;
        tsl.end += zb.bot.d_i + carry;
        zb.main.f += zb.main.d_f;
        carry = (unsigned)zb.main.f < (unsigned)zb.main.d_f;
        tsl.iW += dW_nocarry;
        tsl.Eu += -udW_nocarry;
        tsl.Ev += -vdW_nocarry;
        tsl.start += zb.main.d_i + carry;
        tsl.zstart += zb.main.d_i + carry << 1;
#if LIGHT
        zb.pi.current += zb.pi.d_nocarry;
#endif
        zb.pz.current += zb.pz.d_nocarry;
        if (carry)
        {
            tsl.iW += idWx;
            tsl.Eu += -tsl.udW;
            tsl.Ev += -tsl.vdW;
#if LIGHT
            zb.pi.current += zb.pi.grad_x;
#endif
            zb.pz.current += zb.pz.grad_x;
        }
        if ((signed)tsl.iW < 0)
            return;
        while (tsl.Eu >= (signed)tsl.iW)
        {
            tsl.Eu -= tsl.iW;
            tsl.source = incu(tsl.source, SIZE);
            tsl.udW += tsl.dWx;
            udW_nocarry += dW_nocarry;
        }
        while (tsl.Eu < 0)
        {
            tsl.Eu += tsl.iW;
            tsl.source = decu(tsl.source, SIZE);
            tsl.udW -= tsl.dWx;
            udW_nocarry -= dW_nocarry;
        }
        while (tsl.Ev >= (signed)tsl.iW)
        {
            tsl.Ev -= tsl.iW;
            tsl.source = incv(tsl.source, SIZE);
            tsl.vdW += tsl.dWx;
            vdW_nocarry += dW_nocarry;
        }
        while (tsl.Ev < 0)
        {
            tsl.Ev += tsl.iW;
            tsl.source = decv(tsl.source, SIZE);
            tsl.vdW -= tsl.dWx;
            vdW_nocarry -= dW_nocarry;
        }
        /* End of second innermost loop */
    }
}