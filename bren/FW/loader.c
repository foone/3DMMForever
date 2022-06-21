/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: loader.c 1.1 1995/07/28 19:03:07 sam Exp $
 * $Locker:  $
 *
 * Win32 Image Loader - Can load EXEs and DLLs
 */
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "fw.h"
#include "wincoff.h"

#define READ_BLOCK(var, fh)                                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        if (BrFileRead(&(var), 1, sizeof(var), (fh)) != sizeof(var))                                                   \
            return NULL;                                                                                               \
    } while (0)

/*
 * List of currently loaded images
 */
br_simple_list _BrLoadedImages;

/*
 * Attempts to load an image file into memory
 */
static br_image *ImageLoad(char *name)
{
    void *fh;
    int mode = BR_FS_MODE_BINARY;
    struct msdos_header dos_header;
    struct coff_header coff_header;
    struct nt_optional_header nt_header;
    struct section_header section_header;
    br_uint_32 pe;
    br_image *img;
    br_uint_8 *arena_base;
    int arena_size;
    int i;
    int offset;

    /*
     * Try name as is.
     */
    fh = BrFileOpenRead(name, 0, NULL, &mode);

    /*
     * Try appending ".DLL"
     */
    if (fh == NULL)
    {
        strcpy(_br_scratch_string, name);
        strcat(_br_scratch_string, ".DLL");
        fh = BrFileOpenRead(_br_scratch_string, 0, NULL, &mode);

        /*
         * Try appending ".BDD"
         */
        if (fh == NULL)
        {
            strcpy(_br_scratch_string, name);
            strcat(_br_scratch_string, ".BDD");
            fh = BrFileOpenRead(_br_scratch_string, 0, NULL, &mode);

            if (fh == NULL)
                return NULL;
        }
    }

    /*
     * Read MSDOS header
     */
    READ_BLOCK(dos_header, fh);

    /*
     * Validate MSDOS header
     */
    if (dos_header.magic != 0x5A4D)
        return NULL;

    /*
     * Read PE signature
     */
    if (dos_header.new_header_offset < sizeof(dos_header))
        return NULL;

    BrFileAdvance(dos_header.new_header_offset - sizeof(dos_header), fh);

    READ_BLOCK(pe, fh);

    if (pe != 0x00004550)
        return NULL;

    /*
     * Read base COFF header
     */
    READ_BLOCK(coff_header, fh);

    /*
     * Check machine type
     */
    if (coff_header.machine != IMAGE_FILE_MACHINE_I386)
        return NULL; /* Wrong processor type */

    if (coff_header.flags & IMAGE_FILE_RELOCS_STRIPPED)
        return NULL; /* No base relocation */

    if (!(coff_header.flags & IMAGE_FILE_EXECUTABLE_IMAGE))
        return NULL; /* Not executable file */

    if (!(coff_header.flags & IMAGE_FILE_32BIT_MACHINE))
        return NULL; /* Not 32 bit machine */

    if (coff_header.opt_header_size != sizeof(nt_header))
        return NULL; /* Expecting NT coff file */

    /*
     * Read optional header
     */
    READ_BLOCK(nt_header, fh);

    img = BrResAllocate(NULL, sizeof(*img), BR_MEMORY_IMAGE);

    img->sections = BrResAllocate(img, sizeof(br_image_section) * coff_header.n_sections, BR_MEMORY_IMAGE_SECTIONS);

    /*
     * Read each section header
     */
    for (i = 0; i < coff_header.n_sections; i++)
    {
        READ_BLOCK(section_header, fh);

        /*
         * Make copies of the parts of the header that
         * are needed
         */
        img->sections[i].name = BrMemStrDup(section_header.section_name);
        img->sections[i].mem_offset = section_header.rva;
        img->sections[i].mem_size = section_header.virtual_size;
        img->sections[i].data_size = section_header.data_size;
        img->sections[i].data_offset = section_header.data_offset;
    }

    /*
     * Allocate image arena (aligned)
     */
    arena_size = nt_header.image_size;
    arena_base = BrResAllocate(img, arena_size, BR_MEMORY_IMAGE_ARENA);

    /*
     * Remember current offset into file
     */
    offset = dos_header.new_header_offset + sizeof(pe) + sizeof(coff_header) + sizeof(nt_header) +
             coff_header.n_sections * sizeof(section_header);

    /*
     * Read each section into image arena
     */
    for (i = 0; i < coff_header.n_sections; i++)
    {

        /*
         * Ignore sections that have no raw data
         */
        if ((img->sections[i].data_offset == 0) || (img->sections[i].data_size == 0))
            continue;

        /*
         * make sure we only ever go forwards in file
         */
        if (offset > img->sections[i].data_offset)
            return NULL; /* Section data not in order */

        /*
         * XXX Check that block is within arena
         */

        /*
         * Advance to start of section in file
         */
        BrFileAdvance(img->sections[i].data_offset - offset, fh);

        /*
         * Record pointer to start of section
         */
        img->sections[i].base = arena_base + img->sections[i].mem_offset;

        /*
         * Read the section into memory
         */
        if (BrFileRead(img->sections[i].base, 1, img->sections[i].data_size, fh) != img->sections[i].data_size)
            return NULL;

        offset = img->sections[i].data_offset + img->sections[i].data_size;
    }

    /*
     * Close file
     */
    BrFileClose(fh);

    /*
     * Exports
     */
    if (nt_header.directories[DIRECTORY_EXPORT].size != 0)
    {
        struct export_directory *ed;

        ed = (export_directory *)(arena_base + nt_header.directories[DIRECTORY_EXPORT].rva);

        /*
         * Fill in image structure with info from export directory
         */
        img->identifier = (char *)(arena_base + ed->name);
        img->ordinal_base = ed->ordinal_base;
        img->n_functions = ed->n_entries;
        img->functions = (void **)(arena_base + ed->export_table);
        img->n_names = ed->n_names;
        img->names = (char **)(arena_base + ed->name_table);
        img->name_ordinals = (br_uint_16 *)(arena_base + ed->ordinal_table);

        /*
         * Relocate the tables of pointers
         */
        for (i = 0; i < img->n_functions; i++)
            img->functions[i] = (void *)((br_uint_32)(img->functions[i]) + arena_base);

        for (i = 0; i < img->n_names; i++)
            img->names[i] = (char *)((br_uint_32)(img->names[i]) + arena_base);
    }

    /*
     * Imports
     */
    if (nt_header.directories[DIRECTORY_IMPORT].size != 0)
    {
        struct import_directory *id;
        void **at;
        br_uint_32 *lt;
        br_image *import_img;
        int n, i;

        /*
         * Count number of import DLLs
         */
        n = 0;
        for (id = (import_directory *)(arena_base + nt_header.directories[DIRECTORY_IMPORT].rva); id->lookup_table;
             id++)
            n++;

        /*
         * Allocate a table of pointers to imported DLL names
         */
        img->imports = BrResAllocate(img, sizeof(*img->imports), BR_MEMORY_IMAGE_NAMES);
        img->n_imports = n;

#if 1
        /*
         * For each entry in the directory table...
         */
        for (n = 0, id = (import_directory *)(arena_base + nt_header.directories[DIRECTORY_IMPORT].rva);
             id->lookup_table; id++, n++)
        {

            /*
             * Get a handle onto referenced DLL
             */
            import_img = BrImageReference((char *)(id->name + arena_base));
            if (import_img == NULL)
                return NULL; /* could not find DLL */

            img->imports[n] = import_img;

            /*
             * For each  entry in lookup table
             */
            at = (void **)(arena_base + id->address_table);

            for (lt = (br_uint_32 *)(arena_base + id->lookup_table); *lt; lt++, at++)
            {
                if (*lt & 0x80000000)
                    *at = BrImageLookupOrdinal(import_img, *lt & 0x7fffffff);
                else
                    *at = BrImageLookupName(import_img, (char *)(*lt + arena_base + 2),
                                            *((br_uint_16 *)(*lt + arena_base)));

                if (*at == NULL)
                    return NULL; /* Could not resolve symbol */
            }
        }
#endif
    }

    /*
     * Base Relocation (only if the base needs to be moved)
     */
    if (((br_int_32)arena_base != nt_header.image_base) && (nt_header.directories[DIRECTORY_BASERELOC].size != 0))
    {

        basereloc_header *header;
        br_uint_16 *entry;
        br_uint_8 *fixup;
        br_int_32 delta;
        br_int_16 delta_h, delta_l;

        offset = 0;
        delta = (br_int_32)arena_base - nt_header.image_base;
        delta_h = delta >> 16;
        delta_l = delta & 0xFFFF;

        /*
         * Loop for each block in section
         */
        while (offset < nt_header.directories[DIRECTORY_BASERELOC].size)
        {
            header = (basereloc_header *)(nt_header.directories[DIRECTORY_BASERELOC].rva + arena_base + offset);
            offset += header->size;

            /*
             * Loop for each entry in block
             */
            for (i = 0, entry = (br_uint_16 *)(header + 1);
                 i < (header->size - sizeof(basereloc_header)) / sizeof(br_uint_16); i++, entry++)
            {

                fixup = ENTRY_OFFSET(*entry) + header->rva + arena_base;

                /*
                 * Check fixup is within arena
                 */
                if (fixup >= arena_base + arena_size)
                    return NULL; /* Fixup overrun */

                /*
                 * Apply delta
                 */
                switch (ENTRY_TYPE(*entry))
                {
                case IMAGE_REL_BASED_ABSOLUTE:
                    /* Do nothing */
                    break;

                case IMAGE_REL_BASED_HIGH:
                    *(br_int_16 *)fixup += delta_h;
                    break;

                case IMAGE_REL_BASED_LOW:
                    *(br_int_16 *)fixup += delta_l;
                    break;

                case IMAGE_REL_BASED_HIGHLOW:
                    *(br_int_32 *)fixup += delta;
                    break;

                case IMAGE_REL_BASED_HIGHADJ:

                case IMAGE_REL_BASED_MIPS_JMPADDR:

                default:
                    return NULL; /* Unknown fixup type */
                }
            }
        }
    }

    return img;
}

/*
 * Looks for an image in the currently loaded set
 */
br_image *BR_PUBLIC_ENTRY BrImageFind(char *pattern)
{
    br_image *img;

    /*
     * Look for image in currently loaded set
     */
    BR_FOR_SIMPLELIST(&_BrLoadedImages, img)
    if (NamePatternMatch(pattern, img->identifier))
        return img;

    return NULL;
}

br_image *BR_PUBLIC_ENTRY BrImageReference(char *name)
{
    br_image *img;
    char tmp[256];

#if 0	
	br_exception ex_type;
	void *ex_value;

	if(ex_type = BrExceptionCatch(&ex_value)) {
		SET_ERROR(ex_type, ex_value);
		return NULL;
	}
#endif

    img = BrImageFind(name);

    /*
     * See if image is already loaded...
     */
    if (img)
    {
        img->ref_count++;
        return img;
    }

    img = ImageLoad(name);

    /*
     * Add image to current list
     */
    BR_SIMPLEADDHEAD(&_BrLoadedImages, img);

    return img;
}

/*
 * Find a symbol in an image file
 */
void *BR_PUBLIC_ENTRY BrImageLookupName(br_image *img, char *name, br_uint_32 hint)
{
    int c, limit, base;

    /*
     * See if 'hint' matches
     */
    if (hint < img->n_names && !strcmp(name, img->names[hint]))
        return img->functions[img->name_ordinals[hint]];

    /*
     * Binary search on name table
     */
    limit = img->n_names;
    base = 0;

    while (limit)
    {
        /*
         * Compare with halfway point
         */
        c = strcmp(name, img->names[base + limit / 2]);

        if (c < 0)
        {
            /*
             * Lower
             */
            limit = limit / 2;
        }
        else if (c > 0)
        {
            /*
             * Higher
             */
            base += limit / 2 + 1;
            limit = limit - (limit / 2 + 1);
        }
        else
        {
            /*
             * Hit
             */
            return img->functions[img->name_ordinals[base + limit / 2]];
        }
    }

    /*
     * No match
     */
    return NULL;
}

void *BR_PUBLIC_ENTRY BrImageLookupOrdinal(br_image *img, br_uint_32 ordinal)
{
    ordinal -= img->ordinal_base;

    /*
     * Check that ordinal exists
     */
    if (ordinal > img->n_functions)
        return NULL;

    return img->functions[ordinal];
}

void BR_PUBLIC_ENTRY BrImageDereference(br_image *image)
{
    image->ref_count--;

    if (image->ref_count > 0)
        return;

    /*
     * Resident images are locked in memory
     * because they contain pointers into the
     * program
     */
    if (image->resident)
        return;

    /*
     * Unlink image
     */
    BR_SIMPLEREMOVE(image);

    /*
     * Free resource
     */
    BrResFree(image);
}

void BR_PUBLIC_ENTRY BrImageFree(br_image *image)
{
    int i;

    /*
     * Dereference all referenced DLLs
     */
    for (i = 0; i < image->n_imports; i++)
        BrImageDereference(image->imports[i]);
}

/*
 * Resource destructor
 */
void BR_CALLBACK _BrImageFree(void *res, br_uint_8 res_class, br_size_t size)
{
    BrImageFree(res);
}
