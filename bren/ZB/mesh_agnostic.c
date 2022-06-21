#include <brender.h>
#include <limits.h>
#include "zb.h"

static inline void __VERTEX_TRANSFORM_PROJECT(br_vertex *vp, struct temp_vertex_fixed *tvp)
{
    br_int_32 w = BrFixedMac3(zb.os_model_to_screen.m[0][3], vp->p.v[0], zb.os_model_to_screen.m[1][3], vp->p.v[1],
                              zb.os_model_to_screen.m[2][3], vp->p.v[2]) +
                  zb.os_model_to_screen.m[3][3];

    br_int_32 x;
    {
        br_int_64 __x;
        __x = ((br_uint_64)zb.os_model_to_screen_hi[0] << 32) | zb.os_model_to_screen.m[3][0];
        __x += (br_int_64)zb.os_model_to_screen.m[0][0] * (br_int_64)vp->p.v[0];
        __x += (br_int_64)zb.os_model_to_screen.m[1][0] * (br_int_64)vp->p.v[1];
        __x += (br_int_64)zb.os_model_to_screen.m[2][0] * (br_int_64)vp->p.v[2];
        __x /= w;
        x = __x + fw.vp_ox;
    }

    tvp->v[0] = x;

    br_int_32 y;
    {
        br_int_64 __y;
        __y = ((br_uint_64)zb.os_model_to_screen_hi[1] << 32) | zb.os_model_to_screen.m[3][1];
        __y += (br_int_64)zb.os_model_to_screen.m[0][1] * (br_int_64)vp->p.v[0];
        __y += (br_int_64)zb.os_model_to_screen.m[1][1] * (br_int_64)vp->p.v[1];
        __y += (br_int_64)zb.os_model_to_screen.m[2][1] * (br_int_64)vp->p.v[2];
        __y /= w;
        y = __y + fw.vp_oy;
    }

    tvp->v[1] = y;

    br_uint_32 z;
    {
        br_uint_64 __z;
        __z = ((br_uint_64)zb.os_model_to_screen_hi[2] << 32) | zb.os_model_to_screen.m[3][2];
        __z += (br_int_64)zb.os_model_to_screen.m[0][2] * (br_int_64)vp->p.v[0];
        __z += (br_int_64)zb.os_model_to_screen.m[1][2] * (br_int_64)vp->p.v[1];
        __z += (br_int_64)zb.os_model_to_screen.m[2][2] * (br_int_64)vp->p.v[2];
        __z <<= 15;
        __z /= w;
        z = __z;
    }

    tvp->comp[C_W] = w;
    tvp->v[2] = z;
}

static inline void __VERTEX_TRANSFORM_PROJECT_CHECK(br_vertex *vp, struct temp_vertex_fixed *tvp)
{
    br_int_32 w = BrFixedMac3(zb.os_model_to_screen.m[0][3], vp->p.v[0], zb.os_model_to_screen.m[1][3], vp->p.v[1],
                              zb.os_model_to_screen.m[2][3], vp->p.v[2]) +
                  zb.os_model_to_screen.m[3][3];

    br_int_32 x;
    {
        br_int_64 __x;
        __x = ((br_uint_64)zb.os_model_to_screen_hi[0] << 32) | zb.os_model_to_screen.m[3][0];
        __x += (br_int_64)zb.os_model_to_screen.m[0][0] * (br_int_64)vp->p.v[0];
        __x += (br_int_64)zb.os_model_to_screen.m[1][0] * (br_int_64)vp->p.v[1];
        __x += (br_int_64)zb.os_model_to_screen.m[2][0] * (br_int_64)vp->p.v[2];
        __x /= w;
        x = __x + fw.vp_ox;
    }

    tvp->v[0] = x;

    if (zb.bounds[0] > x)
    {
        zb.bounds[0] = x;
    }
    if (zb.bounds[2] < x)
    {
        zb.bounds[2] = x;
    }

    br_int_32 y;
    {
        br_int_64 __y;
        __y = ((br_uint_64)zb.os_model_to_screen_hi[1] << 32) | zb.os_model_to_screen.m[3][1];
        __y += (br_int_64)zb.os_model_to_screen.m[0][1] * (br_int_64)vp->p.v[0];
        __y += (br_int_64)zb.os_model_to_screen.m[1][1] * (br_int_64)vp->p.v[1];
        __y += (br_int_64)zb.os_model_to_screen.m[2][1] * (br_int_64)vp->p.v[2];
        __y /= w;
        y = __y + fw.vp_oy;
    }

    tvp->v[1] = y;

    if (zb.bounds[1] > y)
    {
        zb.bounds[1] = y;
    }
    if (zb.bounds[3] < y)
    {
        zb.bounds[3] = y;
    }

    br_uint_32 z;
    {
        br_uint_64 __z;
        __z = ((br_uint_64)zb.os_model_to_screen_hi[2] << 32) | zb.os_model_to_screen.m[3][2];
        __z += (br_int_64)zb.os_model_to_screen.m[0][2] * (br_int_64)vp->p.v[0];
        __z += (br_int_64)zb.os_model_to_screen.m[1][2] * (br_int_64)vp->p.v[1];
        __z += (br_int_64)zb.os_model_to_screen.m[2][2] * (br_int_64)vp->p.v[2];
        __z <<= 15;
        __z /= w;
        z = __z;
    }

    tvp->comp[C_W] = w;
    tvp->v[2] = z;
}

br_int_32 BR_ASM_CALL ZbOSFFVGroupCulled_A(br_face *fp, struct temp_face *tfp, int count)
{
    br_uint_32 loop_count = count;
    br_uint_32 group_count = 0;

    do
    {
        br_scalar dot = BrFixedFMac3(fp->n.v[0], fw.eye_m.v[0], fp->n.v[1], fw.eye_m.v[1], fp->n.v[2], fw.eye_m.v[2]);

        if (dot > fp->d)
        {
            tfp->flag = TFF_VISIBLE;
            zb.vertex_counts[fp->vertices[0]] = 1;
            zb.vertex_counts[fp->vertices[1]] = 1;
            zb.vertex_counts[fp->vertices[2]] = 1;
            group_count = 1;
        }
        else
        {
            tfp->flag = 0;
        }

        fp++;
        tfp++;
        loop_count--;
    } while (loop_count > 0);

    return group_count;
}

br_int_32 BR_ASM_CALL ZbOSFFVGroupCulledLit_A(br_face *fp, struct temp_face *tfp, int count)
{
    br_uint_32 loop_count = count;
    br_uint_32 group_count = 0;

    do
    {
        br_scalar dot = BrFixedFMac3(fp->n.v[0], fw.eye_m.v[0], fp->n.v[1], fw.eye_m.v[1], fp->n.v[2], fw.eye_m.v[2]);

        if (dot > fp->d)
        {
            tfp->flag = TFF_VISIBLE;
            zb.vertex_counts[fp->vertices[0]] = 1;
            zb.vertex_counts[fp->vertices[1]] = 1;
            zb.vertex_counts[fp->vertices[2]] = 1;
            group_count = 1;

            tfp->surface = fw.face_surface_fn(&zb.model->prepared_vertices[fp->vertices[0]], fp, 0);
        }
        else
        {
            tfp->flag = 0;
        }

        fp++;
        tfp++;
        loop_count--;
    } while (loop_count > 0);

    return group_count;
}

br_uint_32 BR_ASM_CALL ZbOSTVGroup_A(br_vertex *vp, struct temp_vertex_fixed *tvp, int count, br_uint_8 *countp)
{
    br_uint_32 loop_count = count;
    while (loop_count > 0)
    {
        if (*countp != 0)
        {
            __VERTEX_TRANSFORM_PROJECT(vp, tvp);
        }

        vp++;
        tvp++;
        countp++;
        loop_count--;
    }
    return count;
}

br_uint_32 BR_ASM_CALL ZbOSTVGroupLit_A(br_vertex *vp, struct temp_vertex_fixed *tvp, int count, br_uint_8 *countp)
{
    br_uint_32 loop_count = count;

    while (loop_count > 0)
    {
        if (*countp != 0)
        {
            __VERTEX_TRANSFORM_PROJECT(vp, tvp);
            fw.surface_fn(vp, &vp->n, tvp->comp);
        }

        vp++;
        tvp++;
        countp++;
        loop_count--;
    }

    return count;
}

br_uint_32 BR_ASM_CALL ZbOSTVGroupBC_A(br_vertex *vp, struct temp_vertex_fixed *tvp, int count, br_uint_8 *countp)
{
    br_uint_32 loop_count = count;
    while (loop_count > 0)
    {
        if (*countp != 0)
        {
            __VERTEX_TRANSFORM_PROJECT_CHECK(vp, tvp);
        }

        vp++;
        tvp++;
        countp++;
        loop_count--;
    }
    return count;
}

br_uint_32 BR_ASM_CALL ZbOSTVGroupLitBC_A(br_vertex *vp, struct temp_vertex_fixed *tvp, int count, br_uint_8 *countp)
{
    br_uint_32 loop_count = count;
    while (loop_count > 0)
    {
        if (*countp != 0)
        {
            __VERTEX_TRANSFORM_PROJECT_CHECK(vp, tvp);
            fw.surface_fn(vp, &vp->n, tvp->comp);
        }

        vp++;
        tvp++;
        countp++;
        loop_count--;
    }
    return count;
}

br_uint_32 BR_ASM_CALL ZbOSCopyModelToScreen_A(void)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        br_uint_64 x = ((br_int_64)fw.model_to_screen.m[i][0] * fw.vp_width);
        if ((x >> 32 & 0xFFFF8000) != 0xFFFF8000)
        {
            return -1;
        }
        zb.os_model_to_screen.m[i][0] = x >> 16;

        br_uint_64 y = ((br_int_64)fw.model_to_screen.m[i][1] * fw.vp_height);
        if ((y >> 32 & 0xFFFF8000) != 0xFFFF8000)
        {
            return -1;
        }
        zb.os_model_to_screen.m[i][1] = y >> 16;

        zb.os_model_to_screen.m[i][2] = -fw.model_to_screen.m[i][2];
        zb.os_model_to_screen.m[i][3] = fw.model_to_screen.m[i][3];
    }

    br_uint_64 x = ((br_uint_64)fw.model_to_screen.m[i][0] * fw.vp_width);
    zb.os_model_to_screen.m[i][0] = x;
    zb.os_model_to_screen_hi[0] = x >> 32;

    br_uint_64 y = ((br_uint_64)fw.model_to_screen.m[i][1] * fw.vp_height);
    zb.os_model_to_screen.m[i][1] = y;
    zb.os_model_to_screen_hi[1] = y >> 32;

    br_uint_32 z = fw.model_to_screen.m[i][2];
    zb.os_model_to_screen.m[i][2] = -z >> 16;
    zb.os_model_to_screen_hi[2] = z << 16;

    zb.os_model_to_screen.m[i][3] = fw.model_to_screen.m[i][3];

    return 0;
}