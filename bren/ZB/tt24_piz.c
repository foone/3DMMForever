#include <brender.h>
#include <limits.h>
#include "fwproto.h"
#include "zb.h"

extern int BR_ASM_CALL SafeFixedMac2Div(int, int, int, int, int);

#define swap(type, a, b)                                                                                               \
    {                                                                                                                  \
        type _;                                                                                                        \
        _ = a;                                                                                                         \
        a = b;                                                                                                         \
        b = _;                                                                                                         \
    }

static br_int_32 colour_row_width = 0;

static br_uint_32 temp_b = 0;
static br_uint_32 temp_g = 0;
static br_uint_32 temp_r = 0;

static br_uint_32 p0_offset_x = 0;
static br_uint_32 p0_offset_y = 0;

static br_uint_32 temp_i = 0;
static br_uint_32 temp_u = 0;
static br_uint_32 temp_v = 0;

static screen_scalar vertex_0[2];
static screen_scalar vertex_1[2];
static screen_scalar vertex_2[2];

static br_int_32 g_divisor = 0;

static inline void __TRAPEZOID_PIZ2_RGB_888(struct scan_edge *edge, br_boolean is_forward, br_fixed_ls *z_val,
                                            br_int_32 *fb_index)
{
    br_int_32 endptr = edge->i;

    while (edge->count > 0)
    {
        br_int_32 fb_curr = *fb_index;
        br_uint_32 z_curr = *z_val;
        br_uint_16 *z_ptr = ((br_uint_16 *)zb.depth_buffer) + fb_curr;

        // pixel loop
        if (is_forward)
        {
            while (fb_curr < endptr)
            {
                br_uint_16 z_hi = z_curr >> 16;
                if (*z_ptr > BrFixedToInt(z_curr))
                {
                    *z_ptr = BrFixedToInt(z_curr);
                    zb.colour_buffer[fb_curr * 3 + 0] = BrFixedToInt(temp_r);
                    zb.colour_buffer[fb_curr * 3 + 1] = BrFixedToInt(temp_g);
                    zb.colour_buffer[fb_curr * 3 + 2] = BrFixedToInt(temp_b);
                }
                z_curr += zb.pz.grad_x;
                z_ptr++;
                fb_curr++;
            }
        }
        else
        {
            while (endptr < fb_curr)
            {
                z_curr -= zb.pz.grad_x;
                z_ptr--;
                fb_curr--;
                if (*z_ptr > BrFixedToInt(z_curr))
                {
                    *z_ptr = BrFixedToInt(z_curr);
                    zb.colour_buffer[fb_curr * 3 + 0] = BrFixedToInt(temp_r);
                    zb.colour_buffer[fb_curr * 3 + 1] = BrFixedToInt(temp_g);
                    zb.colour_buffer[fb_curr * 3 + 2] = BrFixedToInt(temp_b);
                }
            }
        }

        br_uint_32 carry;

        edge->f += edge->d_f;
        carry = (br_uint_32)edge->f < (br_uint_32)edge->d_f;
        endptr += edge->d_i + carry;

        zb.main.f += zb.main.d_f;
        carry = (br_uint_32)zb.main.f < (br_uint_32)zb.main.d_f;
        *fb_index += zb.main.d_i + carry;
        if (carry != 0)
        {
            *z_val += zb.pz.d_carry;
        }
        else
        {
            *z_val += zb.pz.d_nocarry;
        }

        edge->count--;
    }
}

static inline void __TRAPEZOID_PIZ2I_RGB_888(struct scan_edge *edge, br_boolean is_forward, br_fixed_ls *z_val,
                                             br_int_32 *fb_index)
{
    br_int_32 endptr = edge->i;

    while (edge->count > 0)
    {
        br_int_32 fb_curr = *fb_index;
        br_uint_32 z_curr = *z_val;
        br_uint_16 *z_ptr = ((br_uint_16 *)zb.depth_buffer) + fb_curr;

        temp_r = zb.pr.current;
        temp_g = zb.pg.current;
        temp_b = zb.pb.current;

        // pixel loop
        if (is_forward)
        {
            while (fb_curr < endptr)
            {
                if (*z_ptr > BrFixedToInt(z_curr))
                {
                    *z_ptr = BrFixedToInt(z_curr);
                    zb.colour_buffer[fb_curr * 3 + 0] = BrFixedToInt(temp_r);
                    zb.colour_buffer[fb_curr * 3 + 1] = BrFixedToInt(temp_g);
                    zb.colour_buffer[fb_curr * 3 + 2] = BrFixedToInt(temp_b);
                }
                z_curr += zb.pz.grad_x;
                temp_r += zb.pr.grad_x;
                temp_g += zb.pg.grad_x;
                temp_b += zb.pb.grad_x;
                z_ptr++;
                fb_curr++;
            }
        }
        else
        {
            while (endptr < fb_curr)
            {
                z_curr -= zb.pz.grad_x;
                temp_r -= zb.pr.grad_x;
                temp_g -= zb.pg.grad_x;
                temp_b -= zb.pb.grad_x;
                z_ptr--;
                fb_curr--;
                if (*z_ptr > BrFixedToInt(z_curr))
                {
                    *z_ptr = BrFixedToInt(z_curr);
                    zb.colour_buffer[fb_curr * 3 + 0] = BrFixedToInt(temp_r);
                    zb.colour_buffer[fb_curr * 3 + 1] = BrFixedToInt(temp_g);
                    zb.colour_buffer[fb_curr * 3 + 2] = BrFixedToInt(temp_b);
                }
            }
        }

        br_uint_32 carry;

        edge->f += edge->d_f;
        carry = (br_uint_32)edge->f < (br_uint_32)edge->d_f;
        endptr += edge->d_i + carry;

        zb.main.f += zb.main.d_f;
        carry = (br_uint_32)zb.main.f < (br_uint_32)zb.main.d_f;
        *fb_index += zb.main.d_i + carry;
        if (carry != 0)
        {
            *z_val += zb.pz.d_carry;
            zb.pr.current += zb.pr.d_carry;
            zb.pg.current += zb.pg.d_carry;
            zb.pb.current += zb.pb.d_carry;
        }
        else
        {
            *z_val += zb.pz.d_nocarry;
            zb.pr.current += zb.pr.d_nocarry;
            zb.pg.current += zb.pg.d_nocarry;
            zb.pb.current += zb.pb.d_nocarry;
        }

        edge->count--;
    }
}

static inline void __SETUP_PI(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1, struct temp_vertex_fixed *v2)
{
    colour_row_width = zb.row_width / 3;

    /*
     * Work out integer vertex values
     * XXX Should really do this at projection time
     */
    vertex_0[0] = v0->v[0] >> 16;
    vertex_1[0] = v1->v[0] >> 16;
    vertex_2[0] = v2->v[0] >> 16;

    vertex_0[1] = v0->v[1] >> 16;
    vertex_1[1] = v1->v[1] >> 16;
    vertex_2[1] = v2->v[1] >> 16;

    if (vertex_0[1] == vertex_2[1])
    {
        return;
    }

    zb.top.x = vertex_1[0] - vertex_0[0];
    zb.top.y = vertex_1[1] - vertex_0[1];
    zb.top.count = zb.top.y;
    if (zb.top.count != 0)
    {
        zb.top.grad = BrFixedDiv(zb.top.x, zb.top.y);
        zb.top.d_f = (zb.top.grad << 16);
        zb.top.d_i = (zb.top.grad >> 16) + colour_row_width;
    }

    // don't need to save deltas
    zb.bot.count = vertex_2[1] - vertex_1[1];
    if (zb.bot.count != 0)
    {
        zb.bot.grad = BrFixedDiv(vertex_2[0] - vertex_1[0], vertex_2[1] - vertex_1[1]);
        zb.bot.d_f = (zb.bot.grad << 16);
        zb.bot.d_i = (zb.bot.grad >> 16) + colour_row_width;
    }

    zb.main.x = vertex_2[0] - vertex_0[0];
    zb.main.y = vertex_2[1] - vertex_0[1];
    zb.main.grad = BrFixedDiv(zb.main.x, zb.main.y);
    zb.main.d_f = (zb.main.grad << 16);
    zb.main.d_i = (zb.main.grad >> 16) + colour_row_width;

    zb.main.f = 0x80000000;
    zb.main.i = (vertex_0[1] * colour_row_width) + vertex_0[0];
    zb.top.f = 0x80000000;
    zb.top.i = (vertex_0[1] * colour_row_width) + vertex_0[0];
    zb.bot.f = 0x80000000;
    zb.bot.i = (vertex_1[1] * colour_row_width) + vertex_1[0];

    g_divisor = zb.top.x * zb.main.y - zb.main.x * zb.top.y;
}

static inline void __PARAM_PI_DIRN(screen_scalar a, screen_scalar b, screen_scalar c, struct scan_parameter *param)
{
    /* Optimal sort of 3 elements? */
    param->grad_x = SafeFixedMac2Div(b - a, zb.main.y, c - a, -zb.top.y, g_divisor);

    param->grad_y = SafeFixedMac2Div(c - a, zb.top.x, b - a, -zb.main.x, g_divisor);

    param->current = a + param->grad_x / 2;
    param->d_nocarry = BrFixedToInt(zb.main.grad) * param->grad_x + param->grad_y;
    param->d_carry = param->d_nocarry + param->grad_x;
}

void BR_ASM_CALL RawTriangle_PIZ2_RGB_888(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                          struct temp_vertex_fixed *v2)
{
    struct temp_vertex_fixed *a, *b, *c;
    a = v0;
    b = v1;
    c = v2;
    if (v0->v[1] > v1->v[1])
    {
        if (v1->v[1] > v2->v[1])
        {
            a = v2;
            c = v0;
        }
        else if (v0->v[1] > v2->v[1])
        {
            a = v1;
            b = v2;
            c = v0;
        }
        else
        {
            a = v1;
            b = v0;
        }
    }
    else if (v1->v[1] > v2->v[1])
    {
        if (v0->v[1] > v2->v[1])
        {
            a = v2;
            b = v0;
            c = v1;
        }
        else
        {
            b = v2;
            c = v1;
        }
    }

    __SETUP_PI(a, b, c);

    if (g_divisor == 0)
    {
        return;
    }

    __PARAM_PI_DIRN(a->v[2], b->v[2], c->v[2], &zb.pz);

    br_boolean is_forward = g_divisor >= 0;
    if (!is_forward)
    {
        g_divisor = -g_divisor;
    }

    temp_r = zb.pr.current;
    temp_g = zb.pg.current;
    temp_b = zb.pb.current;

    br_fixed_ls z_val = zb.pz.current;
    br_int_32 fb_index = zb.main.i;

    __TRAPEZOID_PIZ2_RGB_888(&zb.top, is_forward, &z_val, &fb_index);
    __TRAPEZOID_PIZ2_RGB_888(&zb.bot, is_forward, &z_val, &fb_index);
}

void BR_ASM_CALL RawTriangle_PIZ2I_RGB_888(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                           struct temp_vertex_fixed *v2)
{
    struct temp_vertex_fixed *a, *b, *c;
    a = v0;
    b = v1;
    c = v2;
    if (v0->v[1] > v1->v[1])
    {
        if (v1->v[1] > v2->v[1])
        {
            a = v2;
            c = v0;
        }
        else if (v0->v[1] > v2->v[1])
        {
            a = v1;
            b = v2;
            c = v0;
        }
        else
        {
            a = v1;
            b = v0;
        }
    }
    else if (v1->v[1] > v2->v[1])
    {
        if (v0->v[1] > v2->v[1])
        {
            a = v2;
            b = v0;
            c = v1;
        }
        else
        {
            b = v2;
            c = v1;
        }
    }

    __SETUP_PI(a, b, c);

    if (g_divisor == 0)
    {
        return;
    }

    __PARAM_PI_DIRN(a->v[2], b->v[2], c->v[2], &zb.pz);
    __PARAM_PI_DIRN(a->comp[C_R], b->comp[C_R], c->comp[C_R], &zb.pr);
    __PARAM_PI_DIRN(a->comp[C_G], b->comp[C_G], c->comp[C_G], &zb.pg);
    __PARAM_PI_DIRN(a->comp[C_B], b->comp[C_B], c->comp[C_B], &zb.pb);

    br_boolean is_forward = g_divisor >= 0;
    if (!is_forward)
    {
        g_divisor = -g_divisor;
    }

    br_fixed_ls z_val = zb.pz.current;
    br_int_32 fb_index = zb.main.i;

    __TRAPEZOID_PIZ2I_RGB_888(&zb.top, is_forward, &z_val, &fb_index);
    __TRAPEZOID_PIZ2I_RGB_888(&zb.bot, is_forward, &z_val, &fb_index);
}

void BR_ASM_CALL TriangleRenderPIZ2_RGB_888(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                            struct temp_vertex_fixed *v2)
{
    RawTriangle_PIZ2_RGB_888(v0, v1, v2);
}

void BR_ASM_CALL TriangleRenderPIZ2I_RGB_888(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1,
                                             struct temp_vertex_fixed *v2)
{
    RawTriangle_PIZ2I_RGB_888(v0, v1, v2);
}