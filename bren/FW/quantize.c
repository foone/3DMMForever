/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: quantize.c 1.2 1995/02/22 21:42:27 sam Exp $
 * $Locker:  $
 *
 *
 */

/**********************************************************************
        C Implementation of Wu's Color Quantizer (v. 2)
        (see Graphics Gems vol. II, pp. 126-133)

Author:	Xiaolin Wu
    Dept. of Computer Science
    Univ. of Western Ontario
    London, Ontario N6A 5B7
    wu@csd.uwo.ca

Algorithm: Greedy orthogonal bipartition of RGB space for variance
       minimization aided by inclusion-exclusion tricks.
       For speed no nearest neighbor search is done. Slightly
       better performance can be expected by more sophisticated
       but more expensive versions.

The author thanks Tom Lane at Tom_Lane@G.GP.CS.CMU.EDU for much of
additional documentation and a cure to a previous bug.

Free to distribute, comments and suggestions are appreciated.

Converted by SamL for use in avc
    by SE for use in Brender

**********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "brender.h"

#define MAXCOLOR 256

#define RED 2
#define GREEN 1
#define BLUE 0

struct box
{
    int r0; /* min value, exclusive */
    int r1; /* max value, inclusive */
    int g0;
    int g1;
    int b0;
    int b1;
    int vol;
};

/* Histogram is in elements 1..HISTSIZE along each axis,
 * element 0 is for base or marginal value
 * NB: these must start out 0!
 */

static long wt[33][33][33];
static long mr[33][33][33];
static long mg[33][33][33];
static long mb[33][33][33];
static float m2[33][33][33];

/*
 * Output lookup table
 */
static unsigned char tag[32 * 32 * 32];
static int tag_dist[32 * 32 * 32];

/*
 * color look-up table size
 */
static int K;

/*
 * Fast LUT of n^2 for n = 0..255
 */
static long square_table[256];

/*
 * At conclusion of the histogram step, we can interpret
 *   wt[r][g][b] = sum over voxel of P(c)
 *   mr[r][g][b] = sum over voxel of r*P(c)  ,  similarly for mg, mb
 *   m2[r][g][b] = sum over voxel of c^2*P(c)
 * Actually each of these should be divided by 'size' to give the usual
 * interpretation of P() as ranging from 0 to 1, but we needn't do that here.
 */

/* We now convert histogram into moments so that we can rapidly calculate
 * the sums of the above quantities over any desired box.
 */

static long palette_dist[256];
static long palette_count[256];

/*
 * compute cumulative moments.
 */
static void Moments(void)
{
    unsigned short int ind1, ind2;
    unsigned char i, r, g, b;
    long line, line_r, line_g, line_b;
    long area[33], area_r[33], area_g[33], area_b[33];
    float line2, area2[33];

    for (r = 1; r <= 32; ++r)
    {

        for (i = 0; i <= 32; ++i)
            area2[i] = area[i] = area_r[i] = area_g[i] = area_b[i] = 0;

        for (g = 1; g <= 32; ++g)
        {
            line2 = line = line_r = line_g = line_b = 0;

            for (b = 1; b <= 32; ++b)
            {
                ind1 = (r << 10) + (r << 6) + r + (g << 5) + g + b; /* [r][g][b] */
                line += ((long *)wt)[ind1];
                line_r += ((long *)mr)[ind1];
                line_g += ((long *)mg)[ind1];
                line_b += ((long *)mb)[ind1];
                line2 += ((float *)m2)[ind1];

                area[b] += line;
                area_r[b] += line_r;
                area_g[b] += line_g;
                area_b[b] += line_b;
                area2[b] += line2;

                ind2 = ind1 - 1089; /* [r-1][g][b] */

                ((long *)wt)[ind1] = ((long *)wt)[ind2] + area[b];
                ((long *)mr)[ind1] = ((long *)mr)[ind2] + area_r[b];
                ((long *)mg)[ind1] = ((long *)mg)[ind2] + area_g[b];
                ((long *)mb)[ind1] = ((long *)mb)[ind2] + area_b[b];
                ((float *)m2)[ind1] = ((float *)m2)[ind2] + area2[b];
            }
        }
    }
}

/*
 * Compute sum over a box of any given statistic
 */
static long Vol(struct box *cube, long mmt[33][33][33])
{
    return (mmt[cube->r1][cube->g1][cube->b1] - mmt[cube->r1][cube->g1][cube->b0] - mmt[cube->r1][cube->g0][cube->b1] +
            mmt[cube->r1][cube->g0][cube->b0] - mmt[cube->r0][cube->g1][cube->b1] + mmt[cube->r0][cube->g1][cube->b0] +
            mmt[cube->r0][cube->g0][cube->b1] - mmt[cube->r0][cube->g0][cube->b0]);
}

/*
 * The next two routines allow a slightly more efficient calculation
 * of Vol() for a proposed subbox of a given box.  The sum of Top()
 * and Bottom() is the Vol() of a subbox split in the given direction
 * and with the specified new upper bound.
 */

/*
 * Compute part of Vol(cube, mmt) that doesn't depend on r1, g1, or b1
 * (depending on dir)
 */
static long Bottom(struct box *cube, unsigned char dir, long mmt[33][33][33])
{
    switch (dir)
    {
    case RED:
        return (-mmt[cube->r0][cube->g1][cube->b1] + mmt[cube->r0][cube->g1][cube->b0] +
                mmt[cube->r0][cube->g0][cube->b1] - mmt[cube->r0][cube->g0][cube->b0]);
        break;
    case GREEN:
        return (-mmt[cube->r1][cube->g0][cube->b1] + mmt[cube->r1][cube->g0][cube->b0] +
                mmt[cube->r0][cube->g0][cube->b1] - mmt[cube->r0][cube->g0][cube->b0]);
        break;
    case BLUE:
        return (-mmt[cube->r1][cube->g1][cube->b0] + mmt[cube->r1][cube->g0][cube->b0] +
                mmt[cube->r0][cube->g1][cube->b0] - mmt[cube->r0][cube->g0][cube->b0]);
        break;
    }
    return 0;
}

/*
 * Compute remainder of Vol(cube, mmt), substituting pos for
 * r1, g1, or b1 (depending on dir)
 */
static long Top(struct box *cube, unsigned char dir, int pos, long mmt[33][33][33])
{
    switch (dir)
    {
    case RED:
        return (mmt[pos][cube->g1][cube->b1] - mmt[pos][cube->g1][cube->b0] - mmt[pos][cube->g0][cube->b1] +
                mmt[pos][cube->g0][cube->b0]);
        break;
    case GREEN:
        return (mmt[cube->r1][pos][cube->b1] - mmt[cube->r1][pos][cube->b0] - mmt[cube->r0][pos][cube->b1] +
                mmt[cube->r0][pos][cube->b0]);
        break;
    case BLUE:
        return (mmt[cube->r1][cube->g1][pos] - mmt[cube->r1][cube->g0][pos] - mmt[cube->r0][cube->g1][pos] +
                mmt[cube->r0][cube->g0][pos]);
        break;
    }
    return 0;
}

/*
 * Compute the weighted variance of a box
 * NB: as with the raw statistics, this is really the variance * size
 */
static float Var(struct box *cube)
{
    float dr, dg, db, xx;

    dr = Vol(cube, mr);
    dg = Vol(cube, mg);
    db = Vol(cube, mb);
    xx = m2[cube->r1][cube->g1][cube->b1] - m2[cube->r1][cube->g1][cube->b0] - m2[cube->r1][cube->g0][cube->b1] +
         m2[cube->r1][cube->g0][cube->b0] - m2[cube->r0][cube->g1][cube->b1] + m2[cube->r0][cube->g1][cube->b0] +
         m2[cube->r0][cube->g0][cube->b1] - m2[cube->r0][cube->g0][cube->b0];

    return (xx - (dr * dr + dg * dg + db * db) / (float)Vol(cube, wt));
}

/*
 * We want to minimize the sum of the variances of two subboxes.
 * The sum(c^2) terms can be ignored since their sum over both subboxes
 * is the same (the sum for the whole box) no matter where we split.
 * The remaining terms have a minus sign in the variance formula,
 * so we drop the minus sign and MAXIMIZE the sum of the two terms.
 */

static float Maximize(struct box *cube, unsigned char dir, int first, int last, int *cut, long whole_r, long whole_g,
                      long whole_b, long whole_w)
{
    long half_r, half_g, half_b, half_w;
    long base_r, base_g, base_b, base_w;
    int i;
    float temp, max;

    base_r = Bottom(cube, dir, mr);
    base_g = Bottom(cube, dir, mg);
    base_b = Bottom(cube, dir, mb);
    base_w = Bottom(cube, dir, wt);

    max = 0.0;
    *cut = -1;
    for (i = first; i < last; ++i)
    {
        half_r = base_r + Top(cube, dir, i, mr);
        half_g = base_g + Top(cube, dir, i, mg);
        half_b = base_b + Top(cube, dir, i, mb);
        half_w = base_w + Top(cube, dir, i, wt);

        /*
         * now half_x is sum over lower half of box, if split at i
         */
        if (half_w == 0)
        {
            /*
             * subbox could be empty of pixels!
             * never split into an empty box
             */
            continue;
        }
        else
        {
            temp = ((float)half_r * half_r + (float)half_g * half_g + (float)half_b * half_b) / half_w;
        }

        half_r = whole_r - half_r;
        half_g = whole_g - half_g;
        half_b = whole_b - half_b;
        half_w = whole_w - half_w;

        if (half_w == 0)
        {
            /*
             * subbox could be empty of pixels!
             * never split into an empty box
             */
            continue;
        }
        else
        {
            temp += ((float)half_r * half_r + (float)half_g * half_g + (float)half_b * half_b) / half_w;
        }

        if (temp > max)
        {
            max = temp;
            *cut = i;
        }
    }
    return (max);
}

static int Cut(struct box *set1, struct box *set2)
{
    unsigned char dir;
    int cutr, cutg, cutb;
    float maxr, maxg, maxb;
    long whole_r, whole_g, whole_b, whole_w;

    whole_r = Vol(set1, mr);
    whole_g = Vol(set1, mg);
    whole_b = Vol(set1, mb);
    whole_w = Vol(set1, wt);

    maxr = Maximize(set1, RED, set1->r0 + 1, set1->r1, &cutr, whole_r, whole_g, whole_b, whole_w);
    maxg = Maximize(set1, GREEN, set1->g0 + 1, set1->g1, &cutg, whole_r, whole_g, whole_b, whole_w);
    maxb = Maximize(set1, BLUE, set1->b0 + 1, set1->b1, &cutb, whole_r, whole_g, whole_b, whole_w);

    if ((maxr >= maxg) && (maxr >= maxb))
    {
        dir = RED;
        if (cutr < 0)
            return 0; /* can't split the box */
    }
    else
    {
        if ((maxg >= maxr) && (maxg >= maxb))
            dir = GREEN;
        else
            dir = BLUE;
    }

    set2->r1 = set1->r1;
    set2->g1 = set1->g1;
    set2->b1 = set1->b1;

    switch (dir)
    {
    case RED:
        set2->r0 = set1->r1 = cutr;
        set2->g0 = set1->g0;
        set2->b0 = set1->b0;
        break;
    case GREEN:
        set2->g0 = set1->g1 = cutg;
        set2->r0 = set1->r0;
        set2->b0 = set1->b0;
        break;
    case BLUE:
        set2->b0 = set1->b1 = cutb;
        set2->r0 = set1->r0;
        set2->g0 = set1->g0;
        break;
    }

    set1->vol = (set1->r1 - set1->r0) * (set1->g1 - set1->g0) * (set1->b1 - set1->b0);
    set2->vol = (set2->r1 - set2->r0) * (set2->g1 - set2->g0) * (set2->b1 - set2->b0);
    return 1;
}

static void Mark(struct box *cube, int label)
{
    int r, g, b;

    for (r = cube->r0; r < cube->r1; r++)
        for (g = cube->g0; g < cube->g1; g++)
            for (b = cube->b0; b < cube->b1; b++)
                ((unsigned char *)tag)[(r << 10) + (g << 5) + b] = label;
}

void BR_PUBLIC_ENTRY BrQuantBegin(void)
{
    int i;

    /*
     * make table of squares
     */
    for (i = 0; i < 256; ++i)
        square_table[i] = i * i;

    /*
     * Clear all static workspace
     */
    memset(wt, 0, sizeof(wt));
    memset(mr, 0, sizeof(mr));
    memset(mg, 0, sizeof(mg));
    memset(mb, 0, sizeof(mb));
    memset(m2, 0, sizeof(m2));
    memset(tag, 0, sizeof(tag));
}

void BR_PUBLIC_ENTRY BrQuantEnd(void)
{
}

/*
 * Add an image to quantizing process
 *
 * Accumulate 3-D color histogram of counts, r/g/b, c^2
 */
void BR_PUBLIC_ENTRY BrQuantAddColours(br_uint_8 *colours, br_uint_32 size)
{
    int ind, r, g, b;
    int inr, ing, inb;
    long i;

    for (i = 0; i < size; i++)
    {
        /*
         * Fetch components of pixel
         */
        r = colours[0];
        g = colours[1];
        b = colours[2];
        colours += 3;

        inr = (r >> 3) + 1;
        ing = (g >> 3) + 1;
        inb = (b >> 3) + 1;

        ind = (inr << 10) + (inr << 6) + inr + (ing << 5) + ing + inb;

        ((long *)wt)[ind]++;
        ((long *)mr)[ind] += r;
        ((long *)mg)[ind] += g;
        ((long *)mb)[ind] += b;
        ((float *)m2)[ind] += (float)(square_table[r] + square_table[g] + square_table[b]);
    }
}

void BR_PUBLIC_ENTRY BrQuantMakePalette(int base, int num_entries, br_pixelmap *palette)
{
    struct box cube[MAXCOLOR];
    int next;
    register long int i, weight;
    register int k;
    float vv[MAXCOLOR], temp;
    br_colour colour, *palette_entry;

    if (num_entries > MAXCOLOR)
        BR_ERROR1("Too many palette entries (MAXCOLOR = %d)", MAXCOLOR);

    K = num_entries;

    /*
     * Calculate moments
     */
    Moments();

    /*
     * Partition RGB space
     */

    /*
     * One big partition over whole space
     */
    cube[0].r0 = cube[0].g0 = cube[0].b0 = 0;
    cube[0].r1 = cube[0].g1 = cube[0].b1 = 32;
    next = 0;

    for (i = 1; i < K; ++i)
    {
        if (Cut(&cube[next], &cube[i]))
        {
            /*
             * volume test ensures we won't try to cut one-cell box
             */
            vv[next] = (cube[next].vol > 1) ? Var(&cube[next]) : 0.0;
            vv[i] = (cube[i].vol > 1) ? Var(&cube[i]) : 0.0;
        }
        else
        {
            vv[next] = 0.0; /* don't try to split this box again */
            i--;            /* didn't create box i */
        }

        next = 0;
        temp = vv[0];

        for (k = 1; k <= i; ++k)
        {
            if (vv[k] > temp)
            {
                temp = vv[k];
                next = k;
            }
        }
        if (temp <= 0.0)
        {
            K = i + 1;
            /*
            fprintf(stderr, "Only got %d boxes\n", K);
            */
            break;
        }
    }

    palette_entry = palette->pixels;

    for (k = 0; k < K; k++)
    {
        Mark(&cube[k], k);
        weight = Vol(&cube[k], wt);

        if (weight)
            palette_entry[k + base] = ((Vol(&cube[k], mr) / weight) << 16) + ((Vol(&cube[k], mg) / weight) << 8) +
                                      (Vol(&cube[k], mb) / weight);
    }
}

void BR_PUBLIC_ENTRY BrQuantMapColours(int base, br_uint_8 *colours, br_uint_8 *mapped_colours, int size)
{
    int i;
    int inr, ing, inb;

    for (i = 0; i < size; i++)
    {

        inr = (colours[0] >> 3);
        ing = (colours[1] >> 3);
        inb = (colours[2] >> 3);
        colours += 3;

        *mapped_colours++ = tag[(inr << 10) + (ing << 5) + inb] + base;
    }
}

void BR_PUBLIC_ENTRY BrQuantPrepareMapping(int base, int num_entries, br_pixelmap *palette)
{
    int r, g, b;
    int dr2, dg2;
    br_uint_8 pr, pg, pb;
    int p;
    int s, d;
    int t;
    br_colour *palette_entry;

    /*
     * Initialise RGB cube to max. distances
     */
    for (t = 0; t < 32 * 32 * 32; t++)
    {
        tag_dist[t] = 32767;
        tag[t] = 0;
    }

    /*
     * For each palette entry
     */
    for (p = 0, palette_entry = palette->pixels; p < num_entries; p++, palette_entry++)
    {

        pr = BR_RED(*(palette_entry + base)) >> 3;
        pg = BR_GRN(*(palette_entry + base)) >> 3;
        pb = BR_BLU(*(palette_entry + base)) >> 3;

        /*
         * Walk through RGB cube
         */
        for (r = 0; r < 32; r++)
        {

            d = (int)r - (int)pr;
            dr2 = d * d;

            for (g = 0; g < 32; g++)
            {

                d = (int)g - (int)pg;
                dg2 = d * d;

                for (b = 0; b < 32; b++)
                {

                    /*
                     * If this point is nearer to the current palette
                     * entry, use that instead
                     */
                    t = (r << 10) + (g << 5) + b;

                    d = (int)b - (int)pb;
                    s = dr2 + dg2 + d * d;

                    if (s < tag_dist[t])
                    {
                        tag_dist[t] = s;
                        tag[t] = p;
                    }
                }
            }
        }
    }
}
