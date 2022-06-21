/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: pixelmap.c 1.17 1995/08/31 16:29:37 sam Exp $
 * $Locker:  $
 *
 * Manipulating pixelmaps
 */

#include "fw.h"
#include "brassert.h"

static char rscid[] = "$Id: pixelmap.c 1.17 1995/08/31 16:29:37 sam Exp $";

/*
 * Useful info about each pixelmap type
 */
static struct pm_type_info
{
    /*
     * Size, in bits, of each pixel (including padding)
     */
    br_uint_16 bits;

    /*
     * Size, in bytes, that should be used for saving/loading
     */
    br_uint_16 file_size;

    /*
     * Alignment of rows, in pixels
     */
    br_uint_16 align;

    /*
     * Mask of channels in pixelmap
     */
    br_uint_16 channels;

} pm_type_info[] = {
    {
        1,
        1,
        32,
        BR_PMCHAN_INDEX,
    }, /* BR_PMT_INDEX_1   */
    {
        2,
        1,
        16,
        BR_PMCHAN_INDEX,
    }, /* BR_PMT_INDEX_2   */
    {
        4,
        1,
        8,
        BR_PMCHAN_INDEX,
    }, /* BR_PMT_INDEX_4   */
    {
        8,
        1,
        4,
        BR_PMCHAN_INDEX,
    }, /* BR_PMT_INDEX_8   */

    {
        16,
        2,
        2,
        BR_PMCHAN_RGB,
    }, /* BR_PMT_RGB_555   */
    {
        16,
        2,
        2,
        BR_PMCHAN_RGB,
    }, /* BR_PMT_RGB_565   */
    {
        24,
        3,
        4,
        BR_PMCHAN_RGB,
    }, /* BR_PMT_RGB_888   */
    {
        32,
        4,
        1,
        BR_PMCHAN_RGB,
    }, /* BR_PMT_RGBX_888  */
    {
        32,
        4,
        1,
        BR_PMCHAN_RGB | BR_PMCHAN_ALPHA,
    }, /* BR_PMT_RGBA_8888 */

    {
        16,
        1,
        2,
        BR_PMCHAN_YUV,
    }, /* BR_PMT_YUYV_8888 */
    {
        32,
        1,
        1,
        BR_PMCHAN_YUV,
    }, /* BR_PMT_YUV_888   */

    {
        16,
        2,
        4,
        BR_PMCHAN_DEPTH,
    }, /* BR_PMT_DEPTH_16  */
    {
        32,
        4,
        4,
        BR_PMCHAN_DEPTH,
    }, /* BR_PMT_DEPTH_32  */
    {
        8,
        1,
        4,
        BR_PMCHAN_ALPHA,
    },                                             /* BR_PMT_ALPHA_8	*/
    {16, 2, 2, BR_PMCHAN_INDEX | BR_PMCHAN_ALPHA}, /* BR_PMT_INDEXA_88	*/
};

/*
 * Allocate a new, pixelmap of the given type and size
 *
 * If the 'pixels' pointer is NULL, an appropriate area of memeory will be allocated
 *
 */
br_pixelmap *BR_PUBLIC_ENTRY BrPixelmapAllocate(br_uint_8 type, br_uint_16 w, br_uint_16 h, void *pixels, int flags)
{
    br_pixelmap *pm;
    struct pm_type_info *tip = pm_type_info + type;

    pm = BrResAllocate(fw.res, sizeof(*pm), BR_MEMORY_PIXELMAP);

    UASSERT(type < BR_ASIZE(pm_type_info));

    /*
     * Fill in base structure
     */
    pm->identifier = NULL;
    pm->type = type;
    pm->map = NULL;
    pm->flags = BR_PMF_LINEAR;
    pm->base_x = 0;
    pm->base_y = 0;

    pm->width = w;
    pm->height = h;

    pm->origin_x = 0;
    pm->origin_y = 0;

    /*
     * Work out size of a row
     */
    pm->row_bytes = tip->bits * tip->align * ((w + tip->align - 1) / tip->align) / 8;

    if (((pm->row_bytes * 8) % tip->bits) == 0)
        pm->flags |= BR_PMF_ROW_WHOLEPIXELS;

    /*
     * Allocate pixels
     */
    if (pixels)
        pm->pixels = pixels;
    else
        pm->pixels = BrResAllocate(pm, pm->row_bytes * pm->height, BR_MEMORY_PIXELS);

    /*
     * Make it a bottom up bitmap if required
     */
    if (flags & BR_PMAF_INVERTED)
    {
        pm->pixels = (char *)pm->pixels + pm->row_bytes * (pm->height - 1);
        pm->row_bytes = -pm->row_bytes;
    }

    return pm;
}

/*
 * Create a new pixelmap that references a sub-rectangle of another pixelmap
 */
br_pixelmap *BR_PUBLIC_ENTRY BrPixelmapAllocateSub(br_pixelmap *pm, br_uint_16 x, br_uint_16 y, br_uint_16 w,
                                                   br_uint_16 h)
{
    br_pixelmap *npm;

    /*
     * Create the new structure and copy
     */
    npm = BrResAllocate(fw.res, sizeof(*pm), BR_MEMORY_PIXELMAP);

    *npm = *pm;

    /*
     * Pixel rows are not contiguous
     */
    npm->flags &= ~BR_PMF_LINEAR;

    /*
     * Create sub-window (clipped against original)
     */
    x = (x <= pm->width) ? x : pm->width;
    y = (y <= pm->height) ? y : pm->height;

    npm->base_x += x;
    npm->base_y += y;

    npm->origin_x = 0;
    npm->origin_y = 0;

    npm->width = ((x + w) <= pm->width) ? w : (pm->width - x);
    npm->height = ((y + h) <= pm->height) ? h : (pm->height - y);

    return npm;
}

/*
 * Return the load/save size for a given pixelmap
 */
br_uint_16 BR_PUBLIC_ENTRY BrPixelmapFileSize(br_pixelmap *pm)
{
    ASSERT(pm && (pm->type < BR_ASIZE(pm_type_info)));

    return pm_type_info[pm->type].file_size;
}

/*
 * Return the pixel size in bits
 */
br_uint_16 BR_PUBLIC_ENTRY BrPixelmapPixelSize(br_pixelmap *pm)
{
    ASSERT(pm && (pm->type < BR_ASIZE(pm_type_info)));

    return pm_type_info[pm->type].bits;
}

/*
 * Return a mask of the channels that a pixelmap has
 */
br_uint_16 BR_PUBLIC_ENTRY BrPixelmapChannels(br_pixelmap *pm)
{
    ASSERT(pm && (pm->type < BR_ASIZE(pm_type_info)));

    return pm_type_info[pm->type].channels;
}
