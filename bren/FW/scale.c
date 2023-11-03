/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: scale.c 1.2 1995/02/22 21:42:37 sam Exp $
 * $Locker:  $
 *
 * Image scaling (only RGB)
 * Based on General filtered image rescaling, Dale Schumacher, Graphics Gems III p.8
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "brender.h"
#include "scale.h"

#define CLAMP(a, min, max) (a < min ? min : (a > max ? max : a))

#define BLACK 0x00
#define WHITE 0xff

typedef struct
{
    int pixel;
    float weight;
} CONTRIB;

typedef struct
{
    int n;
    CONTRIB *p;
} CLIST;

static CLIST *contrib;

typedef struct
{
    char red, green, blue;
} RGB3;

void BR_PUBLIC_ENTRY BrScaleBegin(void)
{
}

void BR_PUBLIC_ENTRY BrScaleEnd(void)
{
}

#if 0
float box_filter(float t)
{
    if((t>-0.5) && (t<=0.5))
		return(1.0F);
	else
		return(0.0F);
}
float tri_filter(float t)
{
    if(t<0.0) t = -t;
	if(t<1.0)
		return(1.0F-t);
	else
		return(0.0F);
}
#endif

#define Mitchell_support (2.0F)

#define B (1.0F / 3.0F)
#define C (1.0F / 3.0F)

static float Mitchell_filter(float t)
{
    float tt;

    tt = t * t;
    if (t < 0)
        t = -t;
    if (t < 1.0F)
    {
        t = (((12.0F - 9.0F * B - 6.0F * C) * (t * tt)) + ((-18.0F + 12.0F * B + 6.0F * C) * tt) + (6.0F - 2 * B));
        return (t / 6.0F);
    }
    else if (t < 2.0F)
    {
        t = (((-1.0F * B - 6.0F * C) * (t * tt)) + ((6.0F * B + 30.0F * C) * tt) + ((-12.0F * B - 48.0F * C) * t) +
             (8.0F * B + 24 * C));
        return (t / 6.0F);
    }
    return (0.0F);
}

br_pixelmap *BR_PUBLIC_ENTRY BrPixelmapScale(br_pixelmap *src, br_uint_32 new_x, br_uint_32 new_y, float fwidth)
{
    float xscale, yscale;
    float width, fscale;
    float center, left, right;
    float weight, rw, gw, bw;
    br_pixelmap *temp;
    char *identifier;
    int i, j, k, n;

    char *src_pixels, *pm_pixels;

    if (src->type != BR_PMT_RGB_888)
        BR_ERROR1("'%s' is not a suitable pixelmap to scale", src->identifier);

    identifier = BrMemAllocate(strlen(src->identifier) + 1, BR_MEMORY_STRING);
    strcpy(identifier, src->identifier);

    /*
     * Allocate temp pixelmap for horizontal scale
     */
    temp = BrPixelmapAllocate(BR_PMT_RGB_888, new_x, src->height, NULL, 0);
    xscale = (float)new_x / (double)src->width;
    yscale = (float)new_y / (double)src->height;

    /*
     * Precalculate filter contributions for a row
     */
    contrib = BrMemAllocate(new_x * sizeof(CLIST), BR_MEMORY_SCRATCH);
    if (xscale < 1.0)
    {
        width = fwidth / xscale;
        fscale = 1.0 / xscale;
        for (i = 0; i < new_x; ++i)
        {
            contrib[i].n = 0;
            n = (int)(width * 2 + 1) * sizeof(CONTRIB);
            contrib[i].p = BrMemAllocate(n, BR_MEMORY_SCRATCH);
            center = (float)i / xscale;
            left = ceil(center - width);
            right = floor(center + width);

            for (j = left; j <= right; ++j)
            {
                weight = center - (float)j;
                weight = Mitchell_filter(weight / fscale) / fscale;
                if (j < 0)
                {
                    n = -j;
                }
                else if (j >= src->width)
                {
                    n = (src->width - j) + src->width - 1;
                }
                else
                {
                    n = j;
                }
                k = contrib[i].n++;
                contrib[i].p[k].pixel = n;
                contrib[i].p[k].weight = weight;
            }
        }
    }
    else
    {
        for (i = 0; i < new_x; ++i)
        {
            contrib[i].n = 0;
            contrib[i].p = BrMemAllocate((int)(fwidth * 2 + 1) * sizeof(CONTRIB), BR_MEMORY_SCRATCH);
            center = (float)i / xscale;
            left = ceil(center - fwidth);
            right = floor(center + fwidth);
            for (j = left; j <= right; ++j)
            {
                weight = center - (float)j;
                weight = Mitchell_filter(weight);
                if (j < 0)
                {
                    n = -j;
                }
                else if (j >= src->width)
                {
                    n = (src->width - j) + src->width - 1;
                }
                else
                {
                    n = j;
                }
                k = contrib[i].n++;
                contrib[i].p[k].pixel = n;
                contrib[i].p[k].weight = weight;
            }
        }
    }

    /*
     * Apply filter to zoom horizontally from src to temp
     */

    src_pixels = src->pixels;
    pm_pixels = temp->pixels;

    for (k = 0; k < temp->height; ++k, src_pixels += src->row_bytes, pm_pixels += temp->row_bytes)
    {
        for (i = 0; i < temp->width; ++i)
        {
            rw = bw = gw = 0.0;
            for (j = 0; j < contrib[i].n; ++j)
            {
                rw += src_pixels[0 + 3 * contrib[i].p[j].pixel] * contrib[i].p[j].weight;
                gw += src_pixels[1 + 3 * contrib[i].p[j].pixel] * contrib[i].p[j].weight;
                bw += src_pixels[2 + 3 * contrib[i].p[j].pixel] * contrib[i].p[j].weight;
            }
            pm_pixels[0 + 3 * i] = CLAMP(rw, BLACK, WHITE);
            pm_pixels[1 + 3 * i] = CLAMP(gw, BLACK, WHITE);
            pm_pixels[2 + 3 * i] = CLAMP(bw, BLACK, WHITE);
        }
    }

    /*
     * Free horizontal workspace
     */
    for (i = 0; i < temp->width; i++)
        BrMemFree(contrib[i].p);
    BrMemFree(contrib);

    /*
     * Allocate temp pixelmap for vertical scale
     */
    src = temp;
    temp = BrPixelmapAllocate(BR_PMT_RGB_888, new_x, new_y, NULL, 0);

    /*
     * Precalculate filter contributions for a column
     */
    contrib = BrMemAllocate(new_y * sizeof(CLIST), BR_MEMORY_SCRATCH);
    if (yscale < 1.0)
    {
        width = fwidth / yscale;
        fscale = 1.0 / yscale;
        for (i = 0; i < new_y; ++i)
        {
            contrib[i].n = 0;
            n = (int)(width * 2 + 1) * sizeof(CONTRIB);
            contrib[i].p = BrMemAllocate(n, BR_MEMORY_SCRATCH);
            center = (float)i / yscale;
            left = ceil(center - width);
            right = floor(center + width);
            for (j = left; j <= right; ++j)
            {
                weight = center - (float)j;
                weight = Mitchell_filter(weight / fscale) / fscale;
                if (j < 0)
                {
                    n = -j;
                }
                else if (j >= src->height)
                {
                    n = (src->height - j) + src->height - 1;
                }
                else
                {
                    n = j;
                }
                k = contrib[i].n++;
                contrib[i].p[k].pixel = n;
                contrib[i].p[k].weight = weight;
            }
        }
    }
    else
    {
        for (i = 0; i < new_y; ++i)
        {
            contrib[i].n = 0;
            contrib[i].p = BrMemAllocate((int)(fwidth * 2 + 1) * sizeof(CONTRIB), BR_MEMORY_SCRATCH);
            center = (float)i / yscale;
            left = ceil(center - fwidth);
            right = floor(center + fwidth);
            for (j = left; j <= right; ++j)
            {
                weight = center - (float)j;
                weight = Mitchell_filter(weight);
                if (j < 0)
                {
                    n = -j;
                }
                else if (j >= src->height)
                {
                    n = (src->height - j) + src->height - 1;
                }
                else
                {
                    n = j;
                }
                k = contrib[i].n++;
                contrib[i].p[k].pixel = n;
                contrib[i].p[k].weight = weight;
            }
        }
    }

    /*
     * set src to temp
     * Apply filter to zoom vertically from src to temp
     */

    src_pixels = src->pixels;
    pm_pixels = temp->pixels;

    for (k = 0; k < temp->width; ++k, src_pixels += 3, pm_pixels += 3)
    {
        for (i = 0; i < temp->height; ++i)
        {
            rw = gw = bw = 0.0;
            for (j = 0; j < contrib[i].n; ++j)
            {
                rw += src_pixels[contrib[i].p[j].pixel * src->row_bytes + 0] * contrib[i].p[j].weight;
                gw += src_pixels[contrib[i].p[j].pixel * src->row_bytes + 1] * contrib[i].p[j].weight;
                bw += src_pixels[contrib[i].p[j].pixel * src->row_bytes + 2] * contrib[i].p[j].weight;
            }
            pm_pixels[0 + temp->row_bytes * i] = CLAMP(rw, BLACK, WHITE);
            pm_pixels[1 + temp->row_bytes * i] = CLAMP(gw, BLACK, WHITE);
            pm_pixels[2 + temp->row_bytes * i] = CLAMP(bw, BLACK, WHITE);
        }
    }
    /*
     * Free vertical workspace
     */
    for (i = 0; i < temp->height; i++)
        BrMemFree(contrib[i].p);
    BrMemFree(contrib);

    BrPixelmapFree(src);

    temp->identifier = identifier;

    return temp;
}
