/*
 * Copyright (c) 1993 Argonaut Software Ltd. All rights reserved.
 *
 * $Id: l_piz.c 1.1 1995/08/31 16:52:48 sam Exp $
 * $Locker:  $
 *
 * Mesh rendering to produce points
 */
#include "zb.h"
#include "shortcut.h"
#include "blockops.h"
#include "brassert.h"

static char rscid[] = "$Id: l_piz.c 1.1 1995/08/31 16:52:48 sam Exp $";

#define ABS(x) (((x) < 0) ? -(x) : (x))
#define SWAP(type, a, b)                                                                                               \
    {                                                                                                                  \
        type _;                                                                                                        \
        _ = a;                                                                                                         \
        a = b;                                                                                                         \
        b = _;                                                                                                         \
    }

#define ScalarsToRGB15(r, g, b) (((r) >> 19) & 0x1f | ((g) >> 14) & 0x3e0 | ((b) >> 9) & 0x7c00)

#define ScalarsToRGB16(r, g, b) (((r) >> 19) & 0x1f | ((g) >> 14) & 0x7c0 | ((b) >> 8) & 0xf800)

#define ScalarsToRGB24(r, g, b) (((b) >> 16) & 0xff | ((g) >> 8) & 0xff00 | ((b) >> 8) & 0xff0000)

/*
 * Line drawer - Proper Bresenham algorithm
 */

void BR_ASM_CALL LineRenderPIZ2I(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1)
{
    br_fixed_ls dx, dy;
    br_fixed_ls pm, dm, pz, dz, pi, di, x0, x1, y0, y1;
    int error;
    int count, offset, base;
    int X0, Y0, X1, Y1;
    char *ptr, *zptr;
    int dptr, dzptr;

    x0 = v0->v[X];
    y0 = v0->v[Y];
    x1 = v1->v[X];
    y1 = v1->v[Y];

#if 0
	/*
	 * Clamp ends of line
	 *
 	 * XXX - Should not be necessary!
	 */
	CLAMP_POINT(x0,y0);
	CLAMP_POINT(x1,y1);
#endif

    X0 = BrFixedToInt(x0);
    Y0 = BrFixedToInt(y0);
    X1 = BrFixedToInt(x1);
    Y1 = BrFixedToInt(y1);

    CLAMP_LP(X0, Y0);
    CLAMP_LP(X1, Y1);

    dx = X1 - X0;
    dy = Y1 - Y0;

    error = 0;

    if (ABS(dx) > ABS(dy))
    {
        /*
         * Major axis is X axis - ensure dx is +ve
         */

        if (dx < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = 1 + (dy > 0 ? zb.row_width : -zb.row_width);
        dzptr = 2 + (dy > 0 ? zb.depth_row_width : -zb.depth_row_width);
        dy = ABS(dy);

        if (dx < 0)
        {
            return;
        }

        pz = v0->v[Z];
        pi = v0->comp[C_I];

        if (dx > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dx));
            di = BrFixedDiv((v1->comp[C_I] - v0->comp[C_I]), BrIntToFixed(dx));
        }
        else
        {
            dz = 0;
            di = 0;
        }

        ptr = (char *)zb.colour_buffer + X0 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dx;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                *(unsigned short *)zptr = pz >> 16;
                *ptr = (br_uint_8)ScreenToInt(pi);
            }

            error += dy;
            if (error > 0)
            {
                error -= dx;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr++;
                zptr += 2;
            }

            /*
             * Update parameters
             */
            pi += di;
            pz += dz;
        }
    }
    else
    {
        /*
         * Major axis is Y axis - ensure dy is +ve
         */

        if (dy < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = zb.row_width + (dx > 0 ? 1 : -1);
        dzptr = zb.depth_row_width + (dx > 0 ? 2 : -2);
        dx = ABS(dx);

        if (dy < 0)
            return;

        pz = v0->v[Z];
        pi = v0->comp[C_I];

        if (dy > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dy));
            di = BrFixedDiv((v1->comp[C_I] - v0->comp[C_I]), BrIntToFixed(dy));
        }
        else
        {
            dz = 0;
            di = 0;
        }

        ptr = (char *)zb.colour_buffer + X0 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dy;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                *(unsigned short *)zptr = pz >> 16;
                *ptr = (br_uint_8)ScreenToInt(pi);
            }

            error += dx;
            if (error > 0)
            {
                error -= dy;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr += zb.row_width;
                zptr += zb.depth_row_width;
            }

            /*
             * Update parameters
             */
            pi += di;
            pz += dz;
        }
    }
}

/*
 * Arbitrary size lit textured line drawer
 */
void BR_ASM_CALL LineRenderPIZ2TI(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1)
{
    br_fixed_ls dx, dy;
    br_fixed_ls pm, dm, pz, dz, pi, di, x0, x1, y0, y1;
    br_fixed_ls pu, pv, du, dv;
    int error;
    int count, offset, base;
    int X0, Y0, X1, Y1;
    char *ptr, *zptr;
    int dptr, dzptr;
    int width, height, stride;

    width = zb.material->colour_map->width;
    height = zb.material->colour_map->height;
    stride = zb.material->colour_map->row_bytes;

    x0 = v0->v[X];
    y0 = v0->v[Y];
    x1 = v1->v[X];
    y1 = v1->v[Y];

#if 0
	/*
	 * Clamp ends of line
	 *
 	 * XXX - Should not be necessary!
	 */
	CLAMP_POINT(x0,y0);
	CLAMP_POINT(x1,y1);
#endif

    X0 = BrFixedToInt(x0);
    Y0 = BrFixedToInt(y0);
    X1 = BrFixedToInt(x1);
    Y1 = BrFixedToInt(y1);

    CLAMP_LP(X0, Y0);
    CLAMP_LP(X1, Y1);

    dx = X1 - X0;
    dy = Y1 - Y0;

    error = 0;

    if (ABS(dx) > ABS(dy))
    {
        /*
         * Major axis is X axis - ensure dx is +ve
         */

        if (dx < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = 1 + (dy > 0 ? zb.row_width : -zb.row_width);
        dzptr = 2 + (dy > 0 ? zb.depth_row_width : -zb.depth_row_width);
        dy = ABS(dy);

        if (dx < 0)
            return;

        pz = v0->v[Z];
        pi = v0->comp[C_I];
        pu = v0->comp[C_U] % BrIntToFixed(width);
        if (pu < 0)
            pu += BrIntToFixed(width);
        pv = v0->comp[C_V] % BrIntToFixed(height);
        if (pv < 0)
            pv += BrIntToFixed(height);

        if (dx > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dx));
            di = BrFixedDiv((v1->comp[C_I] - v0->comp[C_I]), BrIntToFixed(dx));
            du = BrFixedDiv((v1->comp[C_U] - v0->comp[C_U]), BrIntToFixed(dx));
            dv = BrFixedDiv((v1->comp[C_V] - v0->comp[C_V]), BrIntToFixed(dx));

            du = du % BrIntToFixed(width);
            if (du > 0)
                du -= BrIntToFixed(width);
            dv = dv % BrIntToFixed(height);
            if (dv > 0)
                dv -= BrIntToFixed(height);
        }

        ptr = (char *)zb.colour_buffer + X0 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dx;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                char texel;
                texel = zb.texture_buffer[(pv >> 16) * stride + (pu >> 16)];

                /* Transparency test */

                if (texel)
                {
                    *(unsigned short *)zptr = pz >> 16;
                    *ptr = zb.shade_table[((pi >> 8) & 0xff00) + texel];
                }
            }

            error += dy;
            if (error > 0)
            {
                error -= dx;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr++;
                zptr += 2;
            }

            /*
             * Update parameters
             */
            pi += di;
            pz += dz;
            pu += du;
            if (pu < 0)
                pu += BrIntToFixed(width);
            pv += dv;
            if (pv < 0)
                pv += BrIntToFixed(height);
        }
    }
    else
    {
        /*
         * Major axis is Y axis - ensure dy is +ve
         */

        if (dy < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = zb.row_width + (dx > 0 ? 1 : -1);
        dzptr = zb.depth_row_width + (dx > 0 ? 2 : -2);
        dx = ABS(dx);

        if (dy < 0)
            return;

        pz = v0->v[Z];
        pi = v0->comp[C_I];
        pu = v0->comp[C_U] % BrIntToFixed(width);
        if (pu < 0)
            pu += BrIntToFixed(width);
        pv = v0->comp[C_V] % BrIntToFixed(height);
        if (pv < 0)
            pv += BrIntToFixed(height);

        if (dy > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dy));
            di = BrFixedDiv((v1->comp[C_I] - v0->comp[C_I]), BrIntToFixed(dy));
            du = BrFixedDiv((v1->comp[C_U] - v0->comp[C_U]), BrIntToFixed(dy));
            dv = BrFixedDiv((v1->comp[C_V] - v0->comp[C_V]), BrIntToFixed(dy));

            du = du % BrIntToFixed(width);
            if (du > 0)
                du -= BrIntToFixed(width);
            dv = dv % BrIntToFixed(height);
            if (dv > 0)
                dv -= BrIntToFixed(height);
        }

        ptr = (char *)zb.colour_buffer + X0 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dy;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                char texel;
                texel = zb.texture_buffer[(pv >> 16) * stride + (pu >> 16)];

                /* Transparency test */

                if (texel)
                {
                    *(unsigned short *)zptr = pz >> 16;
                    *ptr = zb.shade_table[((pi >> 8) & 0xff00) + texel];
                }
            }

            error += dx;
            if (error > 0)
            {
                error -= dy;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr += zb.row_width;
                zptr += zb.depth_row_width;
            }

            /*
             * Update parameters
             */
            pi += di;
            pz += dz;
            pu += du;
            if (pu < 0)
                pu += BrIntToFixed(width);
            pv += dv;
            if (pv < 0)
                pv += BrIntToFixed(height);
        }
    }
}

/*
 * Arbitrary size textured line drawer
 */
void BR_ASM_CALL LineRenderPIZ2T(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1)
{
    br_fixed_ls dx, dy;
    br_fixed_ls pm, dm, pz, dz, x0, x1, y0, y1;
    br_fixed_ls pu, pv, du, dv;
    int error;
    int count, offset, base;
    int X0, Y0, X1, Y1;
    char *ptr, *zptr;
    int dptr, dzptr;
    int width, height, stride;

    width = zb.material->colour_map->width;
    height = zb.material->colour_map->height;
    stride = zb.material->colour_map->row_bytes;

    x0 = v0->v[X];
    y0 = v0->v[Y];
    x1 = v1->v[X];
    y1 = v1->v[Y];

#if 0
	/*
	 * Clamp ends of line
	 *
 	 * XXX - Should not be necessary!
	 */
	CLAMP_POINT(x0,y0);
	CLAMP_POINT(x1,y1);
#endif

    X0 = BrFixedToInt(x0);
    Y0 = BrFixedToInt(y0);
    X1 = BrFixedToInt(x1);
    Y1 = BrFixedToInt(y1);

    CLAMP_LP(X0, Y0);
    CLAMP_LP(X1, Y1);

    dx = X1 - X0;
    dy = Y1 - Y0;

    error = 0;

    if (ABS(dx) > ABS(dy))
    {
        /*
         * Major axis is X axis - ensure dx is +ve
         */

        if (dx < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = 1 + (dy > 0 ? zb.row_width : -zb.row_width);
        dzptr = 2 + (dy > 0 ? zb.depth_row_width : -zb.depth_row_width);
        dy = ABS(dy);

        if (dx < 0)
            return;

        pz = v0->v[Z];
        pu = v0->comp[C_U] % BrIntToFixed(width);
        if (pu < 0)
            pu += BrIntToFixed(width);
        pv = v0->comp[C_V] % BrIntToFixed(height);
        if (pv < 0)
            pv += BrIntToFixed(height);

        if (dx > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dx));
            du = BrFixedDiv((v1->comp[C_U] - v0->comp[C_U]), BrIntToFixed(dx));
            dv = BrFixedDiv((v1->comp[C_V] - v0->comp[C_V]), BrIntToFixed(dx));

            du = du % BrIntToFixed(width);
            if (du > 0)
                du -= BrIntToFixed(width);
            dv = dv % BrIntToFixed(height);
            if (dv > 0)
                dv -= BrIntToFixed(height);
        }

        ptr = (char *)zb.colour_buffer + X0 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dx;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                char texel;
                texel = zb.texture_buffer[(pv >> 16) * stride + (pu >> 16)];

                /* Transparency test */

                if (texel)
                {
                    *(unsigned short *)zptr = pz >> 16;
                    *ptr = texel;
                }
            }

            error += dy;
            if (error > 0)
            {
                error -= dx;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr++;
                zptr += 2;
            }

            /*
             * Update parameters
             */
            pz += dz;
            pu += du;
            if (pu < 0)
                pu += BrIntToFixed(width);
            pv += dv;
            if (pv < 0)
                pv += BrIntToFixed(height);
        }
    }
    else
    {
        /*
         * Major axis is Y axis - ensure dy is +ve
         */

        if (dy < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = zb.row_width + (dx > 0 ? 1 : -1);
        dzptr = zb.depth_row_width + (dx > 0 ? 2 : -2);
        dx = ABS(dx);

        if (dy < 0)
            return;

        pz = v0->v[Z];
        pu = v0->comp[C_U] % BrIntToFixed(width);
        if (pu < 0)
            pu += BrIntToFixed(width);
        pv = v0->comp[C_V] % BrIntToFixed(height);
        if (pv < 0)
            pv += BrIntToFixed(height);

        if (dy > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dy));
            du = BrFixedDiv((v1->comp[C_U] - v0->comp[C_U]), BrIntToFixed(dy));
            dv = BrFixedDiv((v1->comp[C_V] - v0->comp[C_V]), BrIntToFixed(dy));

            du = du % BrIntToFixed(width);
            if (du > 0)
                du -= BrIntToFixed(width);
            dv = dv % BrIntToFixed(height);
            if (dv > 0)
                dv -= BrIntToFixed(height);
        }

        ptr = (char *)zb.colour_buffer + X0 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dy;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                char texel;
                texel = zb.texture_buffer[(pv >> 16) * stride + (pu >> 16)];

                /* Transparency test */

                if (texel)
                {
                    *(unsigned short *)zptr = pz >> 16;
                    *ptr = texel;
                }
            }

            error += dx;
            if (error > 0)
            {
                error -= dy;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr += zb.row_width;
                zptr += zb.depth_row_width;
            }

            /*
             * Update parameters
             */
            pz += dz;
            pu += du;
            if (pu < 0)
                pu += BrIntToFixed(width);
            pv += dv;
            if (pv < 0)
                pv += BrIntToFixed(height);
        }
    }
}

/*
 * line drawer
 */
void BR_ASM_CALL LineRenderPIZ2T_RGB_888(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1)
{
    br_fixed_ls dx, dy;
    br_fixed_ls pm, dm, pz, dz, x0, x1, y0, y1;
    br_fixed_ls pu, pv, du, dv;
    int error;
    int count, offset, base;
    int X0, Y0, X1, Y1;
    char *ptr, *zptr;
    int dptr, dzptr;
    int width, height, stride;

    width = zb.material->colour_map->width;
    height = zb.material->colour_map->height;
    stride = zb.material->colour_map->row_bytes;

    x0 = v0->v[X];
    y0 = v0->v[Y];
    x1 = v1->v[X];
    y1 = v1->v[Y];

#if 0
	/*
	 * Clamp ends of line
	 *
 	 * XXX - Should not be necessary!
	 */
	CLAMP_POINT(x0,y0);
	CLAMP_POINT(x1,y1);
#endif

    X0 = BrFixedToInt(x0);
    Y0 = BrFixedToInt(y0);
    X1 = BrFixedToInt(x1);
    Y1 = BrFixedToInt(y1);

    CLAMP_LP(X0, Y0);
    CLAMP_LP(X1, Y1);

    dx = X1 - X0;
    dy = Y1 - Y0;

    error = 0;

    if (ABS(dx) > ABS(dy))
    {
        /*
         * Major axis is X axis - ensure dx is +ve
         */

        if (dx < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = 3 + (dy > 0 ? zb.row_width : -zb.row_width);
        dzptr = 2 + (dy > 0 ? zb.depth_row_width : -zb.depth_row_width);
        dy = ABS(dy);

        if (dx < 0)
            return;

        pz = v0->v[Z];
        pu = v0->comp[C_U] % BrIntToFixed(width);
        if (pu < 0)
            pu += BrIntToFixed(width);
        pv = v0->comp[C_V] % BrIntToFixed(height);
        if (pv < 0)
            pv += BrIntToFixed(height);

        if (dx > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dx));
            du = BrFixedDiv((v1->comp[C_U] - v0->comp[C_U]), BrIntToFixed(dx));
            dv = BrFixedDiv((v1->comp[C_V] - v0->comp[C_V]), BrIntToFixed(dx));

            du = du % BrIntToFixed(width);
            if (du > 0)
                du -= BrIntToFixed(width);
            dv = dv % BrIntToFixed(height);
            if (dv > 0)
                dv -= BrIntToFixed(height);
        }

        ptr = (char *)zb.colour_buffer + 3 * X0 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dx;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                char *texel;
                texel = zb.texture_buffer + (pv >> 16) * stride + 3 * (pu >> 16);
                if (texel[0] || texel[1] || texel[2])
                {
                    *(unsigned short *)zptr = pz >> 16;
                    ptr[0] = texel[0];
                    ptr[1] = texel[1];
                    ptr[2] = texel[2];
                }
            }

            error += dy;
            if (error > 0)
            {
                error -= dx;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr += 3;
                zptr += 2;
            }

            /*
             * Update parameters
             */
            pz += dz;
            pu += du;
            if (pu < 0)
                pu += BrIntToFixed(width);
            pv += dv;
            if (pv < 0)
                pv += BrIntToFixed(height);
        }
    }
    else
    {
        /*
         * Major axis is Y axis - ensure dy is +ve
         */

        if (dy < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = zb.row_width + (dx > 0 ? 3 : -3);
        dzptr = zb.depth_row_width + (dx > 0 ? 2 : -2);
        dx = ABS(dx);

        if (dy < 0)
            return;

        pz = v0->v[Z];
        pu = v0->comp[C_U] % BrIntToFixed(width);
        if (pu < 0)
            pu += BrIntToFixed(width);
        pv = v0->comp[C_V] % BrIntToFixed(height);
        if (pv < 0)
            pv += BrIntToFixed(height);

        if (dy > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dy));
            du = BrFixedDiv((v1->comp[C_U] - v0->comp[C_U]), BrIntToFixed(dy));
            dv = BrFixedDiv((v1->comp[C_V] - v0->comp[C_V]), BrIntToFixed(dy));

            du = du % BrIntToFixed(width);
            if (du > 0)
                du -= BrIntToFixed(width);
            dv = dv % BrIntToFixed(height);
            if (dv > 0)
                dv -= BrIntToFixed(height);
        }

        ptr = (char *)zb.colour_buffer + X0 * 3 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dy;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                char *texel;

                /* Transparency test */

                texel = zb.texture_buffer + (pv >> 16) * stride + 3 * (pu >> 16);
                if (texel[0] || texel[1] || texel[2])
                {
                    *(unsigned short *)zptr = pz >> 16;
                    ptr[0] = texel[0];
                    ptr[1] = texel[1];
                    ptr[2] = texel[2];
                }
            }

            error += dx;
            if (error > 0)
            {
                error -= dy;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr += zb.row_width;
                zptr += zb.depth_row_width;
            }

            /*
             * Update parameters
             */
            pz += dz;
            pu += du;
            if (pu < 0)
                pu += BrIntToFixed(width);
            pv += dv;
            if (pv < 0)
                pv += BrIntToFixed(height);
        }
    }
}

/*
 * Gouraud shaded 24-bit line drawer
 */
void BR_ASM_CALL LineRenderPIZ2I_RGB_888(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1)
{
    br_fixed_ls dx, dy;
    br_fixed_ls pm, dm, pz, dz, x0, x1, y0, y1;
    br_fixed_ls pr, pg, pb, dr, dg, db;
    int error;
    int count, offset, base;
    int X0, Y0, X1, Y1;
    char *ptr, *zptr;
    int dptr, dzptr;

    x0 = v0->v[X];
    y0 = v0->v[Y];
    x1 = v1->v[X];
    y1 = v1->v[Y];

#if 0
	/*
	 * Clamp ends of line
	 *
 	 * XXX - Should not be necessary!
	 */
	CLAMP_POINT(x0,y0);
	CLAMP_POINT(x1,y1);
#endif

    X0 = BrFixedToInt(x0);
    Y0 = BrFixedToInt(y0);
    X1 = BrFixedToInt(x1);
    Y1 = BrFixedToInt(y1);

    CLAMP_LP(X0, Y0);
    CLAMP_LP(X1, Y1);

    dx = X1 - X0;
    dy = Y1 - Y0;

    error = 0;

    if (ABS(dx) > ABS(dy))
    {
        /*
         * Major axis is X axis - ensure dx is +ve
         */

        if (dx < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = 3 + (dy > 0 ? zb.row_width : -zb.row_width);
        dzptr = 2 + (dy > 0 ? zb.depth_row_width : -zb.depth_row_width);
        dy = ABS(dy);

        if (dx < 0)
            return;

        pz = v0->v[Z];
        pr = v0->comp[C_R];
        pg = v0->comp[C_G];
        pb = v0->comp[C_B];

        if (dx > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dx));
            dr = BrFixedDiv((v1->comp[C_R] - v0->comp[C_R]), BrIntToFixed(dx));
            dg = BrFixedDiv((v1->comp[C_G] - v0->comp[C_G]), BrIntToFixed(dx));
            db = BrFixedDiv((v1->comp[C_B] - v0->comp[C_B]), BrIntToFixed(dx));
        }

        ptr = (char *)zb.colour_buffer + 3 * X0 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dx;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                *(unsigned short *)zptr = pz >> 16;
                ptr[0] = pb >> 16;
                ptr[1] = pg >> 16;
                ptr[2] = pr >> 16;
            }

            error += dy;
            if (error > 0)
            {
                error -= dx;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr += 3;
                zptr += 2;
            }

            /*
             * Update parameters
             */
            pz += dz;
            pr += dr;
            pg += dg;
            pb += db;
        }
    }
    else
    {
        /*
         * Major axis is Y axis - ensure dy is +ve
         */

        if (dy < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = zb.row_width + (dx > 0 ? 3 : -3);
        dzptr = zb.depth_row_width + (dx > 0 ? 2 : -2);
        dx = ABS(dx);

        if (dy < 0)
            return;

        pz = v0->v[Z];
        pr = v0->comp[C_R];
        pg = v0->comp[C_G];
        pb = v0->comp[C_B];

        if (dy > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dy));
            dr = BrFixedDiv((v1->comp[C_R] - v0->comp[C_R]), BrIntToFixed(dy));
            dg = BrFixedDiv((v1->comp[C_G] - v0->comp[C_G]), BrIntToFixed(dy));
            db = BrFixedDiv((v1->comp[C_B] - v0->comp[C_B]), BrIntToFixed(dy));
        }

        ptr = (char *)zb.colour_buffer + X0 * 3 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dy;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                *(unsigned short *)zptr = pz >> 16;
                ptr[0] = pb >> 16;
                ptr[1] = pg >> 16;
                ptr[2] = pr >> 16;
            }

            error += dx;
            if (error > 0)
            {
                error -= dy;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr += zb.row_width;
                zptr += zb.depth_row_width;
            }

            /*
             * Update parameters
             */
            pz += dz;
            pr += dr;
            pg += dg;
            pb += db;
        }
    }
}

/*
 * line drawer
 */
void BR_ASM_CALL LineRenderPIZ2T_RGB_555(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1)
{
    br_fixed_ls dx, dy;
    br_fixed_ls pm, dm, pz, dz, x0, x1, y0, y1;
    br_fixed_ls pu, pv, du, dv;
    int error;
    int count, offset, base;
    int X0, Y0, X1, Y1;
    char *ptr, *zptr;
    int dptr, dzptr;
    int width, height, stride;

    width = zb.material->colour_map->width;
    height = zb.material->colour_map->height;
    stride = zb.material->colour_map->row_bytes;

    x0 = v0->v[X];
    y0 = v0->v[Y];
    x1 = v1->v[X];
    y1 = v1->v[Y];

#if 0
	/*
	 * Clamp ends of line
	 *
 	 * XXX - Should not be necessary!
	 */
	CLAMP_POINT(x0,y0);
	CLAMP_POINT(x1,y1);
#endif

    X0 = BrFixedToInt(x0);
    Y0 = BrFixedToInt(y0);
    X1 = BrFixedToInt(x1);
    Y1 = BrFixedToInt(y1);

    CLAMP_LP(X0, Y0);
    CLAMP_LP(X1, Y1);

    dx = X1 - X0;
    dy = Y1 - Y0;

    error = 0;

    if (ABS(dx) > ABS(dy))
    {
        /*
         * Major axis is X axis - ensure dx is +ve
         */

        if (dx < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = 2 + (dy > 0 ? zb.row_width : -zb.row_width);
        dzptr = 2 + (dy > 0 ? zb.depth_row_width : -zb.depth_row_width);
        dy = ABS(dy);

        if (dx < 0)
            return;

        pz = v0->v[Z];
        pu = v0->comp[C_U] % BrIntToFixed(width);
        if (pu < 0)
            pu += BrIntToFixed(width);
        pv = v0->comp[C_V] % BrIntToFixed(height);
        if (pv < 0)
            pv += BrIntToFixed(height);

        if (dx > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dx));
            du = BrFixedDiv((v1->comp[C_U] - v0->comp[C_U]), BrIntToFixed(dx));
            dv = BrFixedDiv((v1->comp[C_V] - v0->comp[C_V]), BrIntToFixed(dx));

            du = du % BrIntToFixed(width);
            if (du > 0)
                du -= BrIntToFixed(width);
            dv = dv % BrIntToFixed(height);
            if (dv > 0)
                dv -= BrIntToFixed(height);
        }

        ptr = (char *)zb.colour_buffer + 2 * X0 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dx;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                unsigned short texel;
                texel = *(unsigned short *)(zb.texture_buffer + (pv >> 16) * stride + 2 * (pu >> 16));
                if (texel)
                {
                    *(unsigned short *)zptr = pz >> 16;
                    *(unsigned short *)ptr = texel;
                }
            }

            error += dy;
            if (error > 0)
            {
                error -= dx;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr += 2;
                zptr += 2;
            }

            /*
             * Update parameters
             */
            pz += dz;
            pu += du;
            if (pu < 0)
                pu += BrIntToFixed(width);
            pv += dv;
            if (pv < 0)
                pv += BrIntToFixed(height);
        }
    }
    else
    {
        /*
         * Major axis is Y axis - ensure dy is +ve
         */

        if (dy < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = zb.row_width + (dx > 0 ? 2 : -2);
        dzptr = zb.depth_row_width + (dx > 0 ? 2 : -2);
        dx = ABS(dx);

        if (dy < 0)
            return;

        pz = v0->v[Z];
        pu = v0->comp[C_U] % BrIntToFixed(width);
        if (pu < 0)
            pu += BrIntToFixed(width);
        pv = v0->comp[C_V] % BrIntToFixed(height);
        if (pv < 0)
            pv += BrIntToFixed(height);

        if (dy > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dy));
            du = BrFixedDiv((v1->comp[C_U] - v0->comp[C_U]), BrIntToFixed(dy));
            dv = BrFixedDiv((v1->comp[C_V] - v0->comp[C_V]), BrIntToFixed(dy));

            du = du % BrIntToFixed(width);
            if (du > 0)
                du -= BrIntToFixed(width);
            dv = dv % BrIntToFixed(height);
            if (dv > 0)
                dv -= BrIntToFixed(height);
        }

        ptr = (char *)zb.colour_buffer + X0 * 2 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dy;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                unsigned short texel;

                texel = *(unsigned short *)(zb.texture_buffer + (pv >> 16) * stride + 2 * (pu >> 16));
                if (texel)
                {
                    *(unsigned short *)zptr = pz >> 16;
                    *(unsigned short *)ptr = texel;
                }
            }

            error += dx;
            if (error > 0)
            {
                error -= dy;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr += zb.row_width;
                zptr += zb.depth_row_width;
            }

            /*
             * Update parameters
             */
            pz += dz;
            pu += du;
            if (pu < 0)
                pu += BrIntToFixed(width);
            pv += dv;
            if (pv < 0)
                pv += BrIntToFixed(height);
        }
    }
}

/*
 * Gouraud shaded 15-bit line drawer
 */
void BR_ASM_CALL LineRenderPIZ2I_RGB_555(struct temp_vertex_fixed *v0, struct temp_vertex_fixed *v1)
{
    br_fixed_ls dx, dy;
    br_fixed_ls pm, dm, pz, dz, x0, x1, y0, y1;
    br_fixed_ls pr, pg, pb, dr, dg, db;
    int error;
    int count, offset, base;
    int X0, Y0, X1, Y1;
    char *ptr, *zptr;
    int dptr, dzptr;

    x0 = v0->v[X];
    y0 = v0->v[Y];
    x1 = v1->v[X];
    y1 = v1->v[Y];

#if 0
	/*
	 * Clamp ends of line
	 *
 	 * XXX - Should not be necessary!
	 */
	CLAMP_POINT(x0,y0);
	CLAMP_POINT(x1,y1);
#endif

    X0 = BrFixedToInt(x0);
    Y0 = BrFixedToInt(y0);
    X1 = BrFixedToInt(x1);
    Y1 = BrFixedToInt(y1);

    CLAMP_LP(X0, Y0);
    CLAMP_LP(X1, Y1);

    dx = X1 - X0;
    dy = Y1 - Y0;

    error = 0;

    if (ABS(dx) > ABS(dy))
    {
        /*
         * Major axis is X axis - ensure dx is +ve
         */

        if (dx < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = 2 + (dy > 0 ? zb.row_width : -zb.row_width);
        dzptr = 2 + (dy > 0 ? zb.depth_row_width : -zb.depth_row_width);
        dy = ABS(dy);

        if (dx < 0)
            return;

        pz = v0->v[Z];
        pr = v0->comp[C_R];
        pg = v0->comp[C_G];
        pb = v0->comp[C_B];

        if (dx > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dx));
            dr = BrFixedDiv((v1->comp[C_R] - v0->comp[C_R]), BrIntToFixed(dx));
            dg = BrFixedDiv((v1->comp[C_G] - v0->comp[C_G]), BrIntToFixed(dx));
            db = BrFixedDiv((v1->comp[C_B] - v0->comp[C_B]), BrIntToFixed(dx));
        }

        ptr = (char *)zb.colour_buffer + 2 * X0 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dx;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                *(unsigned short *)zptr = pz >> 16;
                *(unsigned short *)ptr = ScalarsToRGB15(pr, pg, pb);
            }

            error += dy;
            if (error > 0)
            {
                error -= dx;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr += 2;
                zptr += 2;
            }

            /*
             * Update parameters
             */
            pz += dz;
            pr += dr;
            pg += dg;
            pb += db;
        }
    }
    else
    {
        /*
         * Major axis is Y axis - ensure dy is +ve
         */

        if (dy < 0)
        {
            SWAP(struct temp_vertex_fixed *, v0, v1);
            SWAP(int, X0, X1);
            SWAP(int, Y0, Y1);
            dx = -dx;
            dy = -dy;
        }

        dptr = zb.row_width + (dx > 0 ? 2 : -2);
        dzptr = zb.depth_row_width + (dx > 0 ? 2 : -2);
        dx = ABS(dx);

        if (dy < 0)
            return;

        pz = v0->v[Z];
        pr = v0->comp[C_R];
        pg = v0->comp[C_G];
        pb = v0->comp[C_B];

        if (dy > 0)
        {
            dz = BrFixedDiv((v1->v[Z] - v0->v[Z]), BrIntToFixed(dy));
            dr = BrFixedDiv((v1->comp[C_R] - v0->comp[C_R]), BrIntToFixed(dy));
            dg = BrFixedDiv((v1->comp[C_G] - v0->comp[C_G]), BrIntToFixed(dy));
            db = BrFixedDiv((v1->comp[C_B] - v0->comp[C_B]), BrIntToFixed(dy));
        }

        ptr = (char *)zb.colour_buffer + X0 * 2 + Y0 * zb.row_width;
        zptr = (char *)zb.depth_buffer + X0 + X0 + Y0 * zb.depth_row_width;

        count = dy;

        while (count-- >= 0)
        {
            /*
             * plot pixel
             */

            if (*(unsigned short *)zptr > (unsigned short)(pz >> 16))
            {
                *(unsigned short *)zptr = pz >> 16;
                *(unsigned short *)ptr = ScalarsToRGB15(pr, pg, pb);
            }

            error += dx;
            if (error > 0)
            {
                error -= dy;
                ptr += dptr;
                zptr += dzptr;
            }
            else
            {
                ptr += zb.row_width;
                zptr += zb.depth_row_width;
            }

            /*
             * Update parameters
             */
            pz += dz;
            pr += dr;
            pg += dg;
            pb += db;
        }
    }
}

