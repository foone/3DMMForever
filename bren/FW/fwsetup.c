/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: fwsetup.c 1.23 1995/08/31 16:29:30 sam Exp $
 * $Locker:  $
 *
 */
#include "fw.h"
#include "datafile.h"
#include <string.h>

static char rscid[] = "$Id: fwsetup.c 1.23 1995/08/31 16:29:30 sam Exp $";

/*
 * Global renderer state
 */
br_framework_state BR_ASM_DATA fw;

/*
 * Various bits of default data
 */
extern br_model _BrDefaultModel;
extern br_material _BrDefaultMaterial;

extern br_diaghandler *_BrDefaultDiagHandler;
extern br_filesystem *_BrDefaultFilesystem;
extern br_allocator *_BrDefaultAllocator;
extern br_file_primitives _BrFilePrimsNull;

/*
 * System resource classes
 */
static br_resource_class SystemResourceClasses[] = {
    /*
     * These two classes have to be initialised by hand
     */
    {"REGISTRY", BR_MEMORY_REGISTRY, NULL},
    {"ANCHOR", BR_MEMORY_ANCHOR, NULL},

    {"RESOURCE_CLASS", BR_MEMORY_RESOURCE_CLASS, NULL},
    {"SCRATCH,", BR_MEMORY_SCRATCH, NULL},
    {"PIXELMAP", BR_MEMORY_PIXELMAP, NULL},
    {"PIXELS", BR_MEMORY_PIXELS, NULL},
    {"VERTICES", BR_MEMORY_VERTICES, NULL},
    {"FACES", BR_MEMORY_FACES, NULL},
    {"GROUPS", BR_MEMORY_GROUPS, NULL},
    {"MODEL", BR_MEMORY_MODEL, NULL},
    {"MATERIAL", BR_MEMORY_MATERIAL, NULL},
    {"MATERIAL_INDEX", BR_MEMORY_MATERIAL_INDEX, NULL},
    {"ACTOR", BR_MEMORY_ACTOR, NULL},
    {"PREPARED_VERTICES", BR_MEMORY_PREPARED_VERTICES, NULL},
    {"PREPARED_FACES", BR_MEMORY_PREPARED_FACES, NULL},
    {"LIGHT", BR_MEMORY_LIGHT, NULL},
    {"CAMERA", BR_MEMORY_CAMERA, NULL},
    {"BOUNDS", BR_MEMORY_BOUNDS, NULL},
    {"CLIP_PLANE", BR_MEMORY_CLIP_PLANE, NULL},
    {"STRING", BR_MEMORY_STRING, NULL},
    {"TRANSFORM", BR_MEMORY_TRANSFORM, NULL},
    {"FILE", BR_MEMORY_FILE, _BrFileFree},
    {"POOL", BR_MEMORY_POOL, NULL},
    {"RENDER_MATERIAL", BR_MEMORY_RENDER_MATERIAL, NULL},
    {"DATAFILE", BR_MEMORY_DATAFILE, NULL},
#if 0
	{"IMAGE",			BR_MEMORY_IMAGE,			_BrImageFree },
	{"IMAGE_ARENA",		BR_MEMORY_IMAGE_ARENA,		NULL },
	{"IMAGE_SECTIONS",	BR_MEMORY_IMAGE_SECTIONS,	NULL },
	{"IMAGE_NAMES",		BR_MEMORY_IMAGE_NAMES,		NULL },
	{"EXCEPTION_HANDLER",BR_MEMORY_EXCEPTION_HANDLER,NULL },
#endif
    {"RENDER_DATA", BR_MEMORY_RENDER_DATA, NULL},
    {"APPLICATION", BR_MEMORY_APPLICATION, NULL},
};

void BR_PUBLIC_ENTRY BrBegin(void)
{
    int i;

    /*
     * Set handlers to use defaults
     */
    if (fw.diag == NULL)
    {
        fw.diag = _BrDefaultDiagHandler;
    }
    if (fw.fsys == NULL)
    {
        fw.fsys = _BrDefaultFilesystem;
    }
    if (fw.mem == NULL)
    {
        fw.mem = _BrDefaultAllocator;
    }

    /*
     * Set up initial state of file writing
     */
    fw.open_mode = BR_FS_MODE_BINARY;

    /*
     * Initialse all registries
     */
    RegistryNew(&fw.reg_models);
    RegistryNew(&fw.reg_materials);
    RegistryNew(&fw.reg_textures);
    RegistryNew(&fw.reg_tables);
    RegistryNew(&fw.reg_resource_classes);

    /*
     * Fake the resource classes that are required to
     * support the registry
     */
    fw.resource_class_index[BR_MEMORY_REGISTRY] = SystemResourceClasses + 0;

    fw.resource_class_index[BR_MEMORY_ANCHOR] = SystemResourceClasses + 1;

    /*
     * Allocate the zero sized base resource instance for
     * the framework
     */
    fw.res = BrResAllocate(NULL, 0, BR_MEMORY_ANCHOR);

    /*
     * Register all the system resource classes
     */
    for (i = 0; i < BR_ASIZE(SystemResourceClasses); i++)
        BrResClassAdd(SystemResourceClasses + i);

    /*
     * Create default model
     */
    fw.default_model = BrResAllocate(fw.res, sizeof(br_model), BR_MEMORY_MODEL);

    *fw.default_model = _BrDefaultModel;
    fw.default_model->prep_flags |= MODUF_REGISTERED;

    /*
     * Create default material
     */
    fw.default_material = &_BrDefaultMaterial;
    fw.default_material->prep_flags |= MATUF_REGISTERED;
}

void BR_PUBLIC_ENTRY BrEnd(void)
{
    /*
     * Free all resources ...
     */
#if 1
    BrResFree(fw.res);
#endif

    /*
     * Clear out fw structure
     */
    memset(&fw, 0, sizeof(fw));
}

/*
 * User functions for setting new errorhandler, filesystem, or allocator
 */
br_diaghandler *BR_PUBLIC_ENTRY BrDiagHandlerSet(br_diaghandler *newdh)
{
    br_diaghandler *old = fw.diag;

    if (newdh == NULL)
        fw.diag = _BrDefaultDiagHandler;
    else
        fw.diag = newdh;

    return old;
}

br_filesystem *BR_PUBLIC_ENTRY BrFilesystemSet(br_filesystem *newfs)
{
    br_filesystem *old = fw.fsys;

    if (newfs == NULL)
        fw.fsys = _BrDefaultFilesystem;
    else
        fw.fsys = newfs;

    return old;
}

br_allocator *BR_PUBLIC_ENTRY BrAllocatorSet(br_allocator *newal)
{
    br_allocator *old = fw.mem;

    if (newal == NULL)
    {
        fw.mem = _BrDefaultAllocator;
    }
    else
    {
        fw.mem = newal;
    }

    return old;
}
