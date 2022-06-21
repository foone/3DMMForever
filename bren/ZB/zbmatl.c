/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: zbmatl.c 1.4 1995/03/01 15:49:47 sam Exp $
 * $Locker:  $
 *
 * Material preparation for Z buffer renderer
 */
#include "zb.h"
#include "shortcut.h"
#include "blockops.h"
#include "brassert.h"

static char rscid[] = "$Id: zbmatl.c 1.4 1995/03/01 15:49:47 sam Exp $";

static br_uint_16 PixelmapHasZeros(br_pixelmap *pm);

/*
 * ZB renderer specific processing for a material
 *
 * Look up the material in the current output set
 */
void ZbMaterialUpdate(br_material *mat, br_uint_16 update_flags)
{
    int i, map_type;
    br_uint_32 flags;
    struct zb_material_type *zbmt;
    struct br_pixelmap *cm;

    map_type = 0;
    flags = mat->flags;

#if 1
    if ((update_flags & (BR_MATU_COLOURMAP | BR_MATU_RENDERING)) == 0)
        return;
#endif

    /*
     * Augment flags with other info from material
     */
    if ((cm = mat->colour_map) != NULL)
    {
        flags |= ZB_MATF_HAS_MAP;
        map_type = cm->type;

        /*
         * Look for square maps that are a power of 2 in size
         */
        if (cm->width == cm->height && !(cm->width & cm->width - 1))
            flags |= ZB_MATF_SQUARE_POW2;

        /*
         * See if the stride is the same as the width
         */
        if ((cm->width * BrPixelmapPixelSize(cm)) / 8 == cm->row_bytes)
            flags |= ZB_MATF_NO_SKIP;

        /*
         * Does the pixelmap have an opacity channel?
         */
        if (BrPixelmapChannels(cm) & BR_PMCHAN_ALPHA)
            flags |= ZB_MATF_MAP_OPACITY;
        //#if 0
        /*
         * Does the image have any 0 pixels?
         */
        if (PixelmapHasZeros(cm))
            flags |= ZB_MATF_MAP_TRANSPARENT;
        //#endif
    }

#if 0
	/*
	 * Is there any transparency?
	 */
	if(mat->opacity != 255)
		flags |= ZB_MATF_TRANSPARENT;
#endif

    /*
     * Is there a screen door lookup table?
     */
    if (mat->screendoor)
        flags |= ZB_MATF_HAS_SCREEN;

    /*
     * Is there a shade table ?
     */
    if (mat->index_shade)
        flags |= ZB_MATF_HAS_SHADE;

    /*
     * Look up material
     */
    zbmt = zb.type->material_types;
    for (i = 0; i < zb.type->nmaterial_types; i++, zbmt++)
    {
        if ((flags & zbmt->flags_mask) == zbmt->flags_cmp)
        {
            /*
             * Check map type
             */
            if (zbmt->map_type && map_type != zbmt->map_type)
                continue;

            /*
             * See if there are any size restrictions
             */
            if (zbmt->width && zbmt->width != cm->width)
                continue;

            if (zbmt->height && zbmt->height != cm->height)
                continue;

            break;
        }
    }

    /*
     * Make sure we found a matching type
     */
    if (i >= zb.type->nmaterial_types)
        BR_ERROR1("ZbMaterialUpdate: Unable to render material '%s'", mat->identifier ? mat->identifier : "???");

    /*
     * Remember pointer to type
     */

    mat->rptr = zbmt;
}

//#if 0
/*
 * Find out if a pixelmap has any zero pixels
 */
static br_uint_16 PixelmapHasZeros(br_pixelmap *pm)
{
    int x, y, bpp;
    char *row_ptr, *pp;

    /*
     * if we can't read the pixels, assume the worst
     */
    if (pm->flags & BR_PMF_NO_ACCESS)
        return 1;

    bpp = BrPixelmapPixelSize(pm);

    /*
     * Work out starting address
     */
    row_ptr = pm->pixels;
    row_ptr = row_ptr + pm->row_bytes * pm->base_y + (pm->base_x * bpp) / 8;

    for (y = 0; y < pm->height; y++)
    {
        pp = row_ptr;

        switch (bpp)
        {
        case 8:
            for (x = 0; x < pm->width; x++, pp++)
                if (*pp == 0)
                    return 1;
            break;
            /*
                    case 16:
                        for(x=0; x < pm->width; x++, pp+=2)
                            if(*(br_uint_16 *)pp == 0)
                                return 1;
                        break;

                    case 24:
                        for(x=0; x < pm->width; x++, pp+=3)
                            if((*(br_uint_32 *)pp & 0xFFFFFF) == 0)
                                return 1;
                        break;

                    case 32:
                        for(x=0; x < pm->width; x++, pp+=4)
                            if(*(br_uint_32 *)pp == 0)
                                return 1;
                        break;
            */
        default:
            /*
             * Unknown pixel size - assume it does have holes
             */
            return 1;
        }
        row_ptr += pm->row_bytes;
    }

    return 0;
}
//#endif

