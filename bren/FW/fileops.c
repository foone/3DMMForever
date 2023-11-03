/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: fileops.c 1.33 1995/08/31 16:29:25 sam Exp $
 * $Locker:  $
 *
 * Per-chunk file operations
 *
 */
#include <stddef.h>

#include "fw.h"
#include "brassert.h"
#include "datafile.h"

static char rscid[] = "$Id: fileops.c 1.33 1995/08/31 16:29:25 sam Exp $";

/*
 * For digging around in points etc - makes things slightly easier to read
 */
#define X 0
#define Y 1
#define Z 2
#define W 3

#define U 0
#define V 1

/**
 ** End Marker
 **/

static int FopRead_END(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    return 1;
}

static int FopWrite_END(br_datafile *df)
{
    df->prims->chunk_write(df, FID_END, 0);

    return 0;
}

/**
 ** File Info
 **/
struct file_info
{
    br_uint_32 type;
    br_uint_32 version;
};

static br_file_enum_member file_type_FM[] = {
    _ENUM_MEMBER(FILE_TYPE_NONE),

    _ENUM_MEMBER(FILE_TYPE_ACTORS), _ENUM_MEMBER(FILE_TYPE_PIXELMAP),  _ENUM_MEMBER(FILE_TYPE_LIGHT),
    _ENUM_MEMBER(FILE_TYPE_CAMERA), _ENUM_MEMBER(FILE_TYPE_MATERIAL),

    _ENUM_MEMBER(FILE_TYPE_MODEL),  _ENUM_MEMBER(FILE_TYPE_ANIMATION), _ENUM_MEMBER(FILE_TYPE_TREE),
};

static _FILE_ENUM(file_type);

#define _STRUCT_NAME struct file_info
static br_file_struct_member file_info_FM[] = {
    _ENUM_32(type, file_type_F),
    _UINT_32(version),
};
static _FILE_STRUCT(file_info);
#undef _STRUCT_NAME

#if 0
static int FopRead_FILE_INFO(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
	struct file_info fi;

	df->prims->struct_read(df,&file_info_F, &fi);

	return fi.type;
}
#endif

static int FopWrite_FILE_INFO(br_datafile *df, br_uint_32 type)
{
    struct file_info fi;

    fi.type = type;
    fi.version = BR_FILE_VERSION;

    df->prims->chunk_write(df, FID_FILE_INFO, df->prims->struct_size(df, &file_info_F, &fi));
    df->prims->struct_write(df, &file_info_F, &fi);

    return 0;
}

/**
 ** Vertices + UV
 **/

#define _STRUCT_NAME struct br_vertex
static br_file_struct_member br_vertex_FM[] = {
    _SCALAR(p.v[X]),
    _SCALAR(p.v[Y]),
    _SCALAR(p.v[Z]),
};
static _FILE_STRUCT(br_vertex);
#undef _STRUCT_NAME

/*
 * Write out the vertices from a model
 */
static int FopWrite_VERTICES(br_datafile *df, br_vertex *vertices, int nvertices)
{
    df->prims->chunk_write(df, FID_VERTICES,
                           df->prims->count_size(df) + nvertices * df->prims->struct_size(df, &br_vertex_F, NULL));
    df->prims->count_write(df, nvertices);
    DfStructWriteArray(df, &br_vertex_F, vertices, nvertices);

    return 0;
}

/*
 * Adds an array of vertices to a model that is on the stack
 *
 */
static int FopRead_VERTICES(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_model *mp = DfTop(DFST_MODEL, 0);

    mp->vertices = BrResAllocate(mp, count * sizeof(br_vertex), BR_MEMORY_VERTICES);
    DfStructReadArray(df, &br_vertex_F, mp->vertices, count);
    mp->nvertices = (br_uint_16)count;

    return 0;
}

/*
 * Builds an array of vertices and pushes it onto the stack
 *
 */
static int FopRead_OLD_VERTICES(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    void *ptr;

    count = length / df->prims->struct_size(df, &br_vertex_F, NULL);

    ptr = BrResAllocate(fw.res, count * sizeof(br_vertex), BR_MEMORY_VERTICES);

    DfStructReadArray(df, &br_vertex_F, ptr, count);

    DfPush(DFST_VERTICES, ptr, count);

    return 0;
}

/**
 ** Per Vertex UV
 **/
#define _STRUCT_NAME struct br_vertex
static br_file_struct_member br_vertex_uv_FM[] = {
    _SCALAR(map.v[0]),
    _SCALAR(map.v[1]),
};
static _FILE_STRUCT(br_vertex_uv);
#undef _STRUCT_NAME

/*
 * Write out an array of vertices and their texture coordinates
 */
static int FopWrite_VERTEX_UV(br_datafile *df, br_vertex *vertices, int nvertices)
{
    df->prims->chunk_write(df, FID_VERTEX_UV,
                           df->prims->count_size(df) + nvertices * df->prims->struct_size(df, &br_vertex_uv_F, NULL));
    df->prims->count_write(df, nvertices);

    DfStructWriteArray(df, &br_vertex_uv_F, vertices, nvertices);

    return 0;
}

/*
 * Read an array of texture components and add it to the model on the stack
 *
 * The incoming array has to be <= the existing vertex array
 */
static int FopRead_VERTEX_UV(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_model *mp = DfTop(DFST_MODEL, 0);

    if (count > mp->nvertices)
        BR_ERROR0("Vertex UV: too many entries");

    DfStructReadArray(df, &br_vertex_uv_F, mp->vertices, count);

    return 0;
}

#define _STRUCT_NAME struct br_vertex
static br_file_struct_member br_old_vertex_uv_FM[] = {
    _SCALAR(p.v[X]), _SCALAR(p.v[Y]), _SCALAR(p.v[Z]), _SCALAR(map.v[0]), _SCALAR(map.v[1]),
};
static _FILE_STRUCT(br_old_vertex_uv);
#undef _STRUCT_NAME

/*
 * Read an array of vertices and push it onto the stack
 */
static int FopRead_OLD_VERTICES_UV(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    void *ptr;

    count = length / df->prims->struct_size(df, &br_old_vertex_uv_F, NULL);

    ptr = BrResAllocate(fw.res, count * sizeof(br_vertex), BR_MEMORY_VERTICES);

    DfStructReadArray(df, &br_old_vertex_uv_F, ptr, count);

    DfPush(DFST_VERTICES, ptr, count);

    return 0;
}

/**
 ** Material Index
 **/
/*
 * Read a material index and push it onto the stack
 */
static int FopRead_MATERIAL_INDEX(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    char name[BR_MAX_NAME];
    br_material **mip;
    br_uint_32 i;

    /*
     * Allocate block for pointers to materials. Include a NULL pointer
     * at entry 0
     */
    mip = BrResAllocate(fw.res, (count + 1) * sizeof(*mip), BR_MEMORY_MATERIAL_INDEX);

    mip[0] = NULL;

    /*
     * Read and lookup each name
     */
    for (i = 1; i < count + 1; i++)
        mip[i] = BrMaterialFind(df->prims->name_read(df, name));

    /*
     * Push index onto stack
     */
    DfPush(DFST_MATERIAL_INDEX, mip, count + 1);

    return 0;
}

/*
 * Write out the material naes from a table of material pointers
 *
 */
static int FopWrite_MATERIAL_INDEX(br_datafile *df, br_material **materials, int nmaterials)
{
    int i, s;

    /*
     * Work out size of chunk
     */
    s = df->prims->count_size(df);
    for (i = 0; i < nmaterials; i++)
        s += df->prims->name_size(df, materials[i]->identifier);

    df->prims->chunk_write(df, FID_MATERIAL_INDEX, s);
    df->prims->count_write(df, nmaterials);

    /*
     * Go through materials table writing out identifiers
     */
    for (i = 0; i < nmaterials; i++)
        df->prims->name_write(df, materials[i]->identifier);

    return 0;
}

/*
 * Read an old-style material index (no count) and push it onto the stack
 */
static int FopRead_OLD_MATERIAL_INDEX(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    char *mblock, *cp;
    int i, num_materials;
    br_material **mip;

    /*
     * Read the index block into temporary buffer
     */
    mblock = BrScratchAllocate(length);

    if (BrFileRead(mblock, 1, length, df->h) != (int)length)
        BR_ERROR0("could not read material index");

    /*
     * Count the number of 0's in buffer (== number of strings as
     * each string is 0 or more chars terminated with 0)
     */
    for (i = 0, cp = mblock, num_materials = 0; i < (int)length; i++)
        if (*cp++ == '\0')
            num_materials++;

    /*
     * Allocate block for pointers to materials
     */
    mip = BrResAllocate(fw.res, num_materials * sizeof(*mip), BR_MEMORY_MATERIAL_INDEX);

    /*
     * Go through table looking up materials
     */
    for (i = 0, cp = mblock; i < num_materials; i++)
    {
        mip[i] = BrMaterialFind(cp);
        while (*cp++)
            ;
    }

    /*
     * Free working buffer
     */
    BrScratchFree(mblock);

    /*
     * Push index onto stack
     */
    DfPush(DFST_MATERIAL_INDEX, mip, num_materials);

    return 0;
}

/**
 ** Faces
 **/
#define _STRUCT_NAME struct br_face
static br_file_struct_member br_face_FM[] = {
    _UINT_16(vertices[0]), _UINT_16(vertices[1]), _UINT_16(vertices[2]), _UINT_16(smoothing), _UINT_8(flags),
};
static _FILE_STRUCT(br_face);
#undef _STRUCT_NAME

/*
 * Read an array of triangular faces and add it to model on the stack
 */
static int FopRead_FACES(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    struct br_model *mp;
    int i;

    /*
     * Get the model and material table
     */
    mp = DfTop(DFST_MODEL, 0);

    mp->faces = BrResAllocate(mp, count * sizeof(br_face), BR_MEMORY_FACES);
    mp->nfaces = (br_uint_16)count;

    /*
     * Read faces
     */
    DfStructReadArray(df, &br_face_F, mp->faces, mp->nfaces);

    /*
     * make sure each face has at least one smoothing group
     */
    for (i = 0; i < mp->nfaces; i++)
        if (mp->faces[i].smoothing == 0)
            mp->faces[i].smoothing = (br_uint_16)~0;

    return 0;
}

/*
 * Write out an array of faces
 */
static int FopWrite_FACES(br_datafile *df, br_face *faces, int nfaces)
{
    df->prims->chunk_write(df, FID_FACES,
                           df->prims->count_size(df) + nfaces * df->prims->struct_size(df, &br_face_F, NULL));
    df->prims->count_write(df, nfaces);

    DfStructWriteArray(df, &br_face_F, faces, nfaces);

    return 0;
}

/**
 ** Obselete Faces (before 16 bit smoothing values)
 **/
#define _STRUCT_NAME struct br_face
static br_file_struct_member br_old_face_1_FM[] = {
    _UINT_16(vertices[0]), _UINT_16(vertices[1]), _UINT_16(vertices[2]), _UINT_8(smoothing), _UINT_8(flags),
};
static _FILE_STRUCT(br_old_face_1);
#undef _STRUCT_NAME

/*
 * Read an array of triangular faces and add it to model on the stack
 */
static int FopRead_OLD_FACES_1(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    struct br_model *mp;
    int i;

    /*
     * Get the model and material table
     */
    mp = DfTop(DFST_MODEL, 0);

    mp->faces = BrResAllocate(mp, count * sizeof(br_face), BR_MEMORY_FACES);
    mp->nfaces = (br_uint_16)count;

    /*
     * Read faces
     */
    DfStructReadArray(df, &br_old_face_1_F, mp->faces, mp->nfaces);

    /*
     * Convert smoothing groups to bitmask
     */
    for (i = 0; i < mp->nfaces; i++)
    {
        if (mp->faces[i].smoothing == 0)
            mp->faces[i].smoothing = (br_uint_16)~0;
        else
            mp->faces[i].smoothing = 1 << ((mp->faces[i].smoothing - 1) % 16);
    }

    return 0;
}

/*
 * Old faces
 */
#define _STRUCT_NAME struct br_face
static br_file_struct_member br_old_face_FM[] = {
    _UINT_16(vertices[0]), _UINT_16(vertices[1]), _UINT_16(vertices[2]),

    _UINT_16(material),    _UINT_32(smoothing),
};
static _FILE_STRUCT(br_old_face);
#undef _STRUCT_NAME

static int FopRead_OLD_FACES(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    struct br_face *fp;
    br_material **mip;
    int mi_count;
    unsigned int i;
    void *ptr;

    count = length / df->prims->struct_size(df, &br_old_face_F, NULL);

    /*
     * Get the number of vertices and material table
     */
    ptr = DfPop(DFST_VERTICES, (int *)&i);
    mip = DfPop(DFST_MATERIAL_INDEX, &mi_count);
    DfPush(DFST_VERTICES, ptr, i);

    fp = BrResAllocate(fw.res, count * sizeof(br_face), BR_MEMORY_FACES);

    DfStructReadArray(df, &br_old_face_F, fp, count);

    /*
     * Go through and convert materials to pointers
     */
    for (i = 0; i < count; i++)
    {
        fp[i].material = mip[*((br_uint_16 *)&fp[i].material)];

        /*
         * make sure each face has at least one smoothing group
         */
        if (fp[i].smoothing == 0)
            fp[i].smoothing = (br_uint_16)~0;
    }

    /*
     * Free up material index
     */
    BrResFree(mip);

    /*
     * Leave allocated block on stack
     */
    DfPush(DFST_FACES, fp, count);

    return 0;
}

/**
 ** Per face material indices
 **/

static int FopWrite_FACE_MATERIAL(br_datafile *df, br_face *faces, int nfaces, br_material **mindex, int nmaterials)
{
    br_uint_16 *block, *ip;
    br_face *fp;
    int i, j;

    /*
     * Make a block in memory for index
     */
    block = BrResAllocate(fw.res, nfaces * sizeof(*block), BR_MEMORY_MATERIAL_INDEX);

    /*
     * Build index
     */
    for (i = 0, fp = faces, ip = block; i < nfaces; i++, fp++, ip++)
    {
        *ip = 0;

        /*
         * Look up material in index if not NULL
         */
        if (fp->material)
        {
            for (j = 0; j < nmaterials; j++)
            {
                if (mindex[j] == fp->material)
                {
                    *ip = j + 1; /* Index entries start at 1 */
                    break;
                }
            }
        }
    }

    df->prims->chunk_write(df, FID_FACE_MATERIAL, df->prims->block_size(df, block, nfaces, 0, 1, sizeof(br_uint_16)));

    /*
     * Write index as a block
     */
    df->prims->block_write(df, block, nfaces, 0, 1, sizeof(br_uint_16));

    BrResFree(block);

    return 0;
}

static int FopRead_FACE_MATERIAL(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_material **mindex;
    int nmaterials;
    br_model *mp;
    br_face *fp;
    br_uint_16 *block, *ip;
    int block_count, i;

    /*
     * Grab material index of stack
     */
    mindex = DfPop(DFST_MATERIAL_INDEX, &nmaterials);

    mp = DfTop(DFST_MODEL, 0);

    /*
     * Read block from file
     */
    block = BrScratchAllocate(length);
    block_count = mp->nfaces;
    block = df->prims->block_read(df, block, &block_count, sizeof(br_uint_16), 0);

    if (block_count > mp->nfaces)
        BR_ERROR0("Face Materials: too many entries");
    /*
     * Go through block looking up material pointers
     */
    for (i = 0, fp = mp->faces, ip = block; i < block_count; i++, fp++, ip++)
    {
        fp->material = (*ip < nmaterials) ? mindex[*ip] : NULL;
    }
    /*
     * Release block
     */
    BrScratchFree(block);

    /*
     * Release material index
     */
    BrResFree(mindex);

    return 0;
}

/**
 ** Model
 **/
#define _STRUCT_NAME struct br_model
static br_file_struct_member br_model_FM[] = {
    _UINT_16(flags),
    _ASCIZ(identifier),
};
static _FILE_STRUCT(br_model);
#undef _STRUCT_NAME

static int FopRead_MODEL(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_model *mp;

    /*
     * Allocate and read in model structure
     */
    mp = BrModelAllocate(NULL, 0, 0);
    df->res = mp;
    df->prims->struct_read(df, &br_model_F, mp);
    df->res = NULL;

    /*
     * Make sure CUSTOM flag is _NOT_ set
     */
    mp->flags &= ~(BR_MODF_CUSTOM | BR_MODF_PREPREPARED);

    /*
     * Leave model on stack
     */
    DfPush(DFST_MODEL, mp, 1);

    return 0;
}

static int FopWrite_MODEL(br_datafile *df, br_model *mp)
{
    br_model temp_model = *mp;

    /*
     * Don't save the CUSTOM flag
     */
    temp_model.flags &= ~(BR_MODF_CUSTOM | BR_MODF_PREPREPARED);

    df->prims->chunk_write(df, FID_MODEL, df->prims->struct_size(df, &br_model_F, &temp_model));
    df->prims->struct_write(df, &br_model_F, &temp_model);

    return 0;
}

/**
 ** Old model
 **/
#define _STRUCT_NAME struct br_model
static br_file_struct_member br_old_model_1_FM[] = {
    _ASCIZ(identifier),
};
static _FILE_STRUCT(br_old_model_1);
#undef _STRUCT_NAME

static int FopRead_OLD_MODEL_1(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_model *mp;

    /*
     * Allocate and read in model structure
     */
    mp = BrModelAllocate(NULL, 0, 0);
    df->res = mp;
    df->prims->struct_read(df, &br_old_model_1_F, mp);
    df->res = NULL;

    /*
     * Leave model on stack
     */
    DfPush(DFST_MODEL, mp, 1);

    return 0;
}

static int FopRead_OLD_MODEL(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_model *mp;
    int i;

    /*
     * Allocate and read in model structure
     */
    mp = BrModelAllocate(NULL, 0, 0);

    df->res = mp;
    df->prims->struct_read(df, &br_model_F, mp);
    df->res = NULL;

    /*
     * Hook up vertices and faces
     */
    mp->faces = DfPop(DFST_FACES, &i);
    mp->nfaces = i;
    mp->vertices = DfPop(DFST_VERTICES, &i);
    mp->nvertices = i;

    BrResAdd(mp, mp->faces);
    BrResAdd(mp, mp->vertices);

    /*
     * Make sure CUSTOM flag is _NOT_ set
     */
    mp->flags &= ~(BR_MODF_CUSTOM | BR_MODF_PREPREPARED);

    /*
     * Leave model on stack
     */
    DfPush(DFST_MODEL, mp, 1);

    return 0;
}

/**
 ** Pivot
 **/
#define _STRUCT_NAME struct br_model
static br_file_struct_member br_pivot_FM[] = {
    _SCALAR(pivot.v[X]),
    _SCALAR(pivot.v[Y]),
    _SCALAR(pivot.v[Z]),
};
static _FILE_STRUCT(br_pivot);
#undef _STRUCT_NAME

static int FopRead_PIVOT(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_model *mp;

    mp = DfPop(DFST_MODEL, 0);
    df->res = mp;
    df->prims->struct_read(df, &br_pivot_F, mp);
    df->res = NULL;
    DfPush(DFST_MODEL, mp, 1);

    return 0;
}

/**
 ** Material
 **/
#define _STRUCT_NAME struct br_material
static br_file_struct_member br_material_FM[] = {
    _COLOUR(colour),
    _UINT_8(opacity),
    _UFRACTION(ka),
    _UFRACTION(kd),
    _UFRACTION(ks),
    _SCALAR(power),
    _UINT_16(flags),
    _VECTOR2(map_transform.m[0]),
    _VECTOR2(map_transform.m[1]),
    _VECTOR2(map_transform.m[2]),
    _UINT_8(index_base),
    _UINT_8(index_range),
    _ASCIZ(identifier),
};

static _FILE_STRUCT(br_material);
#undef _STRUCT_NAME

static int FopRead_MATERIAL(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_material *mp;

    /*
     * Allocate and read in material structure
     */
    mp = BrMaterialAllocate(NULL);
    df->res = mp;
    df->prims->struct_read(df, &br_material_F, mp);
    df->res = NULL;

    /*
     * Leave material on stack
     */
    DfPush(DFST_MATERIAL, mp, 1);

    return 0;
}

static int FopWrite_MATERIAL(br_datafile *df, br_material *mp)
{
    df->prims->chunk_write(df, FID_MATERIAL, df->prims->struct_size(df, &br_material_F, mp));
    df->prims->struct_write(df, &br_material_F, mp);

    return 0;
}

/*
 * Add a reference to a pixelmap to material on the stack
 */

/*
 * Table of chunk id's versus offsets in material structure
 */
static struct
{
    br_uint_32 id;
    size_t offset;
    int table;
} MaterialMaps[] = {
    {FID_COLOUR_MAP_REF, offsetof(struct br_material, colour_map), 0},
    {FID_INDEX_BLEND_REF, offsetof(struct br_material, index_blend), 1},
    {FID_INDEX_SHADE_REF, offsetof(struct br_material, index_shade), 1},
    {FID_SCREENDOOR_REF, offsetof(struct br_material, screendoor), 1},
};

static int FopRead_PIXELMAP_REF(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_pixelmap *pm;
    char name[BR_MAX_NAME];
    char *mp;
    int i;

    /*
     * Lookup ID in table
     */
    for (i = 0; i < BR_ASIZE(MaterialMaps); i++)
        if (id == MaterialMaps[i].id)
            break;

    ASSERT(i < BR_ASIZE(MaterialMaps));

    /*
     * Get material from stack, get name, and look it up
     */
    mp = DfTop(DFST_MATERIAL, 0);

    if (MaterialMaps[i].table)
        pm = BrTableFind(df->prims->name_read(df, name));
    else
        pm = BrMapFind(df->prims->name_read(df, name));

    *(br_pixelmap **)(mp + MaterialMaps[i].offset) = pm;

    return 0;
}

/*
 * Wrtie out a reference to a pixelmap
 */
static int FopWrite_PIXELMAP_REF(br_datafile *df, int id, br_pixelmap *pixelmap)
{
    ASSERT(pixelmap && pixelmap->identifier);

    df->prims->chunk_write(df, id, df->prims->name_size(df, pixelmap->identifier));
    df->prims->name_write(df, pixelmap->identifier);
    return 0;
}

/**
 ** Pixelmap
 **/
static br_file_enum_member pixelmap_type_FM[] = {
    _ENUM_MEMBER(BR_PMT_INDEX_1),   _ENUM_MEMBER(BR_PMT_INDEX_2),  _ENUM_MEMBER(BR_PMT_INDEX_4),
    _ENUM_MEMBER(BR_PMT_INDEX_8),   _ENUM_MEMBER(BR_PMT_RGB_555),  _ENUM_MEMBER(BR_PMT_RGB_565),
    _ENUM_MEMBER(BR_PMT_RGB_888),   _ENUM_MEMBER(BR_PMT_RGBX_888), _ENUM_MEMBER(BR_PMT_RGBA_8888),
    _ENUM_MEMBER(BR_PMT_YUYV_8888), _ENUM_MEMBER(BR_PMT_YUV_888),  _ENUM_MEMBER(BR_PMT_DEPTH_16),
    _ENUM_MEMBER(BR_PMT_DEPTH_32),  _ENUM_MEMBER(BR_PMT_ALPHA_8),  _ENUM_MEMBER(BR_PMT_INDEXA_88),
};

static _FILE_ENUM(pixelmap_type);

#define _STRUCT_NAME struct br_pixelmap
static br_file_struct_member br_pixelmap_FM[] = {
    _ENUM_8(type, pixelmap_type_F),
    _UINT_16(row_bytes),
    _UINT_16(width),
    _UINT_16(height),
    _UINT_16(origin_x),
    _UINT_16(origin_y),
    _ASCIZ(identifier),
};

static _FILE_STRUCT(br_pixelmap);
#undef _STRUCT_NAME

static int FopWrite_PIXELMAP(br_datafile *df, br_pixelmap *pp)
{
    br_pixelmap pixelmap = *pp;

    pixelmap.row_bytes = pixelmap.width * BrPixelmapPixelSize(&pixelmap) / 8;

    df->prims->chunk_write(df, FID_PIXELMAP, df->prims->struct_size(df, &br_pixelmap_F, &pixelmap));
    df->prims->struct_write(df, &br_pixelmap_F, &pixelmap);

    return 0;
}

static int FopRead_PIXELMAP(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_pixelmap *pp;

    /*
     * Allocate and read in pixelmap structure
     */
    pp = BrResAllocate(fw.res, sizeof(*pp), BR_MEMORY_PIXELMAP);
    df->res = pp;
    df->prims->struct_read(df, &br_pixelmap_F, pp);
    df->res = NULL;

    pp->row_bytes = pp->width * BrPixelmapPixelSize(pp) / 8;

    /*
     * Leave material on stack
     */
    DfPush(DFST_PIXELMAP, pp, 1);

    return 0;
}

/**
 ** Pixel bytes
 **/
static int FopWrite_PIXELS(br_datafile *df, br_pixelmap *pixelmap)
{
    int size = BrPixelmapFileSize(pixelmap);
    int bpp = BrPixelmapPixelSize(pixelmap);
    int block_count = (pixelmap->width * bpp / 8) / size;
    char *pixels;
    int y;

    pixels = (char *)(pixelmap->pixels) + pixelmap->base_y * pixelmap->row_bytes + (pixelmap->base_x * bpp) / 8;

    df->prims->chunk_write(df, FID_PIXELS,
                           df->prims->block_size(df, pixels, block_count, pixelmap->row_bytes, pixelmap->height, size));

    df->prims->block_write(df, pixels, block_count, pixelmap->row_bytes, pixelmap->height, size);

    return 0;
}

static int FopRead_PIXELS(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    int icount;
    br_pixelmap *pp;
    int size;

    pp = DfTop(DFST_PIXELMAP, 0);
    size = BrPixelmapFileSize(pp);

    df->res = pp;
    pp->pixels = df->prims->block_read(df, NULL, &icount, size, BR_MEMORY_PIXELS);
    pp->flags |= BR_PMF_LINEAR;
    df->res = NULL;

    return 0;
}

/**
 ** Connecting a map to an indexed pixelmap
 **/
static int FopWrite_ADD_MAP(br_datafile *df)
{
    df->prims->chunk_write(df, FID_ADD_MAP, 0);

    return 0;
}

static int FopRead_ADD_MAP(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_pixelmap *pp, *map;

    map = DfPop(DFST_PIXELMAP, 0);
    pp = DfTop(DFST_PIXELMAP, 0);

    pp->map = map;

    return 0;
}

/**
 ** Actor
 **/

static br_file_enum_member actor_type_FM[] = {
    _ENUM_MEMBER(BR_ACTOR_NONE),           _ENUM_MEMBER(BR_ACTOR_MODEL),      _ENUM_MEMBER(BR_ACTOR_LIGHT),
    _ENUM_MEMBER(BR_ACTOR_CAMERA),         _ENUM_MEMBER(_BR_ACTOR_RESERVED),  _ENUM_MEMBER(BR_ACTOR_BOUNDS),
    _ENUM_MEMBER(BR_ACTOR_BOUNDS_CORRECT), _ENUM_MEMBER(BR_ACTOR_CLIP_PLANE),
};

static _FILE_ENUM(actor_type);

static br_file_enum_member render_style_FM[] = {
    _ENUM_MEMBER(BR_RSTYLE_DEFAULT),        _ENUM_MEMBER(BR_RSTYLE_NONE),
    _ENUM_MEMBER(BR_RSTYLE_POINTS),         _ENUM_MEMBER(BR_RSTYLE_EDGES),
    _ENUM_MEMBER(BR_RSTYLE_FACES),          _ENUM_MEMBER(BR_RSTYLE_BOUNDING_POINTS),
    _ENUM_MEMBER(BR_RSTYLE_BOUNDING_EDGES), _ENUM_MEMBER(BR_RSTYLE_BOUNDING_FACES),
};

static _FILE_ENUM(render_style);

#define _STRUCT_NAME struct br_actor
static br_file_struct_member br_actor_FM[] = {
    _ENUM_8(type, actor_type_F),
    _ENUM_8(render_style, render_style_F),
    _ASCIZ(identifier),
};
static _FILE_STRUCT(br_actor);
#undef _STRUCT_NAME

static int FopWrite_ACTOR(br_datafile *df, br_actor *ap)
{
    df->prims->chunk_write(df, FID_ACTOR, df->prims->struct_size(df, &br_actor_F, ap));
    df->prims->struct_write(df, &br_actor_F, ap);

    return 0;
}

static int FopRead_ACTOR(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_actor *ap;

    /*
     * Allocate and read in actor structure
     */
    ap = BrActorAllocate(BR_ACTOR_NONE, NULL);
    df->res = ap;
    df->prims->struct_read(df, &br_actor_F, ap);
    df->res = NULL;

    ap->t.type = BR_TRANSFORM_IDENTITY;

    /*
     * Leave actor on stack
     */
    DfPush(DFST_ACTOR, ap, 1);

    return 0;
}

/**
 ** Actor's reference to a model
 **/
static int FopWrite_ACTOR_MODEL(br_datafile *df, br_model *model)
{
    ASSERT(model && model->identifier);

    df->prims->chunk_write(df, FID_ACTOR_MODEL, df->prims->name_size(df, model->identifier));
    df->prims->name_write(df, model->identifier);
    return 0;
}

static int FopRead_ACTOR_MODEL(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    char name[BR_MAX_NAME];
    br_actor *a;

    /*
     * Get actor from stack
     */
    a = DfTop(DFST_ACTOR, 0);

    a->model = BrModelFind(df->prims->name_read(df, name));

    return 0;
}

/**
 ** Actor's reference to a material
 **/
static int FopWrite_ACTOR_MATERIAL(br_datafile *df, br_material *material)
{
    ASSERT(material && material->identifier);

    df->prims->chunk_write(df, FID_ACTOR_MATERIAL, df->prims->name_size(df, material->identifier));
    df->prims->name_write(df, material->identifier);
    return 0;
}

static int FopRead_ACTOR_MATERIAL(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    char name[BR_MAX_NAME];
    br_actor *a;

    /*
     * Get actor from stack
     */
    a = DfTop(DFST_ACTOR, 0);

    a->material = BrMaterialFind(df->prims->name_read(df, name));

    return 0;
}

/**
 ** Actor's transform
 **/
static int FopWrite_ACTOR_TRANSFORM(br_datafile *df)
{
    df->prims->chunk_write(df, FID_ACTOR_TRANSFORM, 0);

    return 0;
}

static int FopRead_ACTOR_TRANSFORM(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_actor *a;
    br_transform *tp;

    /*
     * Get transform and actor of the stack
     */
    tp = DfPop(DFST_TRANSFORM, 0);
    a = DfTop(DFST_ACTOR, 0);

    /*
     * Copy transform into actor
     */
    a->t = *tp;

    /*
     * Free the translation
     */
    BrResFree(tp);

    return 0;
}

/**
 ** Add light to actor
 **/

static int FopWrite_ACTOR_LIGHT(br_datafile *df)
{
    df->prims->chunk_write(df, FID_ACTOR_LIGHT, 0);

    return 0;
}

static int FopRead_ACTOR_LIGHT(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_actor *a;
    br_light *lp;

    /*
     * Get light and actor of the stack
     */
    lp = DfPop(DFST_LIGHT, 0);
    a = DfTop(DFST_ACTOR, 0);

    /*
     * Attach light to actor
     */
    a->type_data = lp;

    return 0;
}

/**
 ** Add camera to actor
 **/

static int FopWrite_ACTOR_CAMERA(br_datafile *df)
{
    df->prims->chunk_write(df, FID_ACTOR_CAMERA, 0);

    return 0;
}

static int FopRead_ACTOR_CAMERA(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_actor *a;
    br_light *cp;

    /*
     * Get camera and actor of the stack
     */
    cp = DfPop(DFST_CAMERA, 0);
    a = DfTop(DFST_ACTOR, 0);

    /*
     * Attach camera to actor
     */
    a->type_data = cp;

    return 0;
}

/**
 ** Add bounds to actor
 **/

static int FopWrite_ACTOR_BOUNDS(br_datafile *df)
{
    df->prims->chunk_write(df, FID_ACTOR_BOUNDS, 0);

    return 0;
}

static int FopRead_ACTOR_BOUNDS(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_actor *a;
    br_bounds *bp;

    /*
     * Get bounds and actor of the stack
     */
    bp = DfPop(DFST_BOUNDS, 0);
    a = DfTop(DFST_ACTOR, 0);

    /*
     * Attach bounds to actor
     */
    a->type_data = bp;

    return 0;
}

/**
 ** Add Clip plane to actor
 **/

static int FopWrite_ACTOR_CLIP_PLANE(br_datafile *df)
{
    df->prims->chunk_write(df, FID_ACTOR_CLIP_PLANE, 0);

    return 0;
}

static int FopRead_ACTOR_CLIP_PLANE(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_actor *a;
    br_vector4 *vp;

    /*
     * Get clip vector and actor of the stack
     */
    vp = DfPop(DFST_PLANE, 0);
    a = DfTop(DFST_ACTOR, 0);

    /*
     * Attach bounds to actor
     */
    a->type_data = vp;

    return 0;
}

static int FopWrite_ACTOR_ADD_CHILD(br_datafile *df)
{
    df->prims->chunk_write(df, FID_ACTOR_ADD_CHILD, 0);

    return 0;
}

/*
 * Add actor at top of stack as a child of actor next on stack
 */
static int FopRead_ACTOR_ADD_CHILD(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_actor *a, *p;

    a = DfPop(DFST_ACTOR, 0);
    p = DfTop(DFST_ACTOR, 0);

    BrActorAdd(p, a);

    return 0;
}

/**
 ** Transforms
 **/
#define _STRUCT_NAME struct br_transform
static br_file_struct_member br_transform_matrix34_FM[] = {
    /*
     * XXX Assumes rows of matrix can be cast to vectors
     */
    _VECTOR3(t.mat.m[0]),
    _VECTOR3(t.mat.m[1]),
    _VECTOR3(t.mat.m[2]),
    _VECTOR3(t.mat.m[3]),
};

static _FILE_STRUCT(br_transform_matrix34);
#undef _STRUCT_NAME

#define _STRUCT_NAME struct br_transform
static br_file_struct_member br_transform_quat_FM[] = {
    _SCALAR(t.quat.q.x), _SCALAR(t.quat.q.y), _SCALAR(t.quat.q.z), _SCALAR(t.quat.q.w),

    _VECTOR3(t.quat.t),
};

static _FILE_STRUCT(br_transform_quat);
#undef _STRUCT_NAME

static br_file_enum_member angle_order_FM[] = {
    _ENUM_MEMBER(BR_EULER_XYZ_S), _ENUM_MEMBER(BR_EULER_XYX_S), _ENUM_MEMBER(BR_EULER_XZY_S),
    _ENUM_MEMBER(BR_EULER_XZX_S), _ENUM_MEMBER(BR_EULER_YZX_S), _ENUM_MEMBER(BR_EULER_YZY_S),
    _ENUM_MEMBER(BR_EULER_YXZ_S), _ENUM_MEMBER(BR_EULER_YXY_S), _ENUM_MEMBER(BR_EULER_ZXY_S),
    _ENUM_MEMBER(BR_EULER_ZXZ_S), _ENUM_MEMBER(BR_EULER_ZYX_S), _ENUM_MEMBER(BR_EULER_ZYZ_S),

    _ENUM_MEMBER(BR_EULER_ZYX_R), _ENUM_MEMBER(BR_EULER_XYX_R), _ENUM_MEMBER(BR_EULER_YZX_R),
    _ENUM_MEMBER(BR_EULER_XZX_R), _ENUM_MEMBER(BR_EULER_XZY_R), _ENUM_MEMBER(BR_EULER_YZY_R),
    _ENUM_MEMBER(BR_EULER_ZXY_R), _ENUM_MEMBER(BR_EULER_YXY_R), _ENUM_MEMBER(BR_EULER_YXZ_R),
    _ENUM_MEMBER(BR_EULER_ZXZ_R), _ENUM_MEMBER(BR_EULER_XYZ_R), _ENUM_MEMBER(BR_EULER_ZYZ_R),
};

static _FILE_ENUM(angle_order);

#define _STRUCT_NAME struct br_transform
static br_file_struct_member br_transform_euler_FM[] = {
    _ENUM_8(t.euler.e.order, angle_order_F),
    _ANGLE(t.euler.e.a),
    _ANGLE(t.euler.e.b),
    _ANGLE(t.euler.e.c),

    _VECTOR3(t.euler.t),
};

static _FILE_STRUCT(br_transform_euler);
#undef _STRUCT_NAME

#define _STRUCT_NAME struct br_transform
static br_file_struct_member br_transform_look_up_FM[] = {
    _VECTOR3(t.look_up.look),
    _VECTOR3(t.look_up.up),
    _VECTOR3(t.look_up.t),
};

static _FILE_STRUCT(br_transform_look_up);
#undef _STRUCT_NAME

#define _STRUCT_NAME struct br_transform
static br_file_struct_member br_transform_translation_FM[] = {
    _VECTOR3(t.translate.t),
};

static _FILE_STRUCT(br_transform_translation);
#undef _STRUCT_NAME

/*
 * Index (by transform type), of chunk ID and file struct to use
 */
static struct transform_type
{
    int id;
    struct br_file_struct *fs;
} TransformTypes[] = {
    {FID_TRANSFORM_MATRIX34, &br_transform_matrix34_F},
    {FID_TRANSFORM_MATRIX34_LP, &br_transform_matrix34_F},
    {FID_TRANSFORM_QUAT, &br_transform_quat_F},
    {FID_TRANSFORM_EULER, &br_transform_euler_F},
    {FID_TRANSFORM_LOOK_UP, &br_transform_look_up_F},
    {FID_TRANSFORM_TRANSLATION, &br_transform_translation_F},
    {FID_TRANSFORM_IDENTITY, NULL},
};

static int FopWrite_TRANSFORM(br_datafile *df, br_transform *t)
{
    struct transform_type *tt;

    ASSERT(t->type < BR_ASIZE(TransformTypes));

    tt = TransformTypes + t->type;

    if (tt->fs)
    {
        df->prims->chunk_write(df, tt->id, df->prims->struct_size(df, tt->fs, t));
        df->prims->struct_write(df, tt->fs, t);
    }
    else
    {
        df->prims->chunk_write(df, tt->id, 0);
    }

    return 0;
}

static int FopRead_TRANSFORM(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    int t;
    br_transform *tp;

    /*
     * Look up ID in table
     */
    for (t = 0; t < BR_ASIZE(TransformTypes); t++)
        if (TransformTypes[t].id == id)
            break;

    ASSERT(t < BR_ASIZE(TransformTypes));

    /*
     * Allocate transform and fill in type
     */
    tp = BrResAllocate(fw.res, sizeof(*tp), BR_MEMORY_TRANSFORM);
    tp->type = t;

    df->res = tp;

    if (TransformTypes[t].fs)
        df->prims->struct_read(df, TransformTypes[t].fs, tp);

    df->res = NULL;

    /*
     * Leave transform on stack
     */
    DfPush(DFST_TRANSFORM, tp, 1);

    return 0;
}

/**
 ** Bounds
 **/

#define _STRUCT_NAME struct br_bounds3
static br_file_struct_member br_bounds3_FM[] = {
    _VECTOR3(min),
    _VECTOR3(max),
};

static _FILE_STRUCT(br_bounds3);
#undef _STRUCT_NAME

static int FopWrite_BOUNDS(br_datafile *df, br_bounds *bp)
{
    df->prims->chunk_write(df, FID_BOUNDS, df->prims->struct_size(df, &br_bounds3_F, bp));
    df->prims->struct_write(df, &br_bounds3_F, bp);

    return 0;
}

int FopRead_BOUNDS(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_bounds3 *bp;

    /*
     * Allocate and read in bounds structure
     */
    bp = BrResAllocate(fw.res, sizeof(*bp), BR_MEMORY_BOUNDS);
    df->res = bp;
    df->prims->struct_read(df, &br_bounds3_F, bp);
    df->res = NULL;

    /*
     * Leave bounds on stack
     */
    DfPush(DFST_BOUNDS, bp, 1);

    return 0;
}

/**
 ** Plane
 **/
struct br_plane
{
    br_vector4 *eqn;
};

#define _STRUCT_NAME struct br_plane
static br_file_struct_member br_plane_FM[] = {
    _VECTOR4(eqn),
};

static _FILE_STRUCT(br_plane);
#undef _STRUCT_NAME

static int FopWrite_PLANE(br_datafile *df, br_vector4 *pp)
{
    df->prims->chunk_write(df, FID_PLANE, df->prims->struct_size(df, &br_plane_F, pp));
    df->prims->struct_write(df, &br_plane_F, pp);

    return 0;
}

static int FopRead_PLANE(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_vector4 *pp;

    /*
     * Allocate and read in vector
     */
    pp = BrResAllocate(fw.res, sizeof(*pp), BR_MEMORY_CLIP_PLANE);
    df->res = pp;
    df->prims->struct_read(df, &br_plane_F, pp);
    df->res = NULL;

    /*
     * Leave bounds on stack
     */
    DfPush(DFST_PLANE, pp, 1);

    return 0;
}

/**
 ** Light
 **/
static br_file_enum_member light_type_FM[] = {
    _ENUM_MEMBER(BR_LIGHT_POINT),
    _ENUM_MEMBER(BR_LIGHT_DIRECT),
    _ENUM_MEMBER(BR_LIGHT_SPOT),

    _ENUM_MEMBER(BR_LIGHT_VIEW | BR_LIGHT_POINT),
    _ENUM_MEMBER(BR_LIGHT_VIEW | BR_LIGHT_DIRECT),
    _ENUM_MEMBER(BR_LIGHT_VIEW | BR_LIGHT_SPOT),
};

static _FILE_ENUM(light_type);

#define _STRUCT_NAME struct br_light
static br_file_struct_member br_light_FM[] = {
    _ENUM_8(type, light_type_F),

    _COLOUR(colour),

    _SCALAR(attenuation_c),
    _SCALAR(attenuation_l),
    _SCALAR(attenuation_q),

    _ANGLE(cone_inner),
    _ANGLE(cone_outer),

    _ASCIZ(identifier),
};
static _FILE_STRUCT(br_light);
#undef _STRUCT_NAME

static int FopWrite_LIGHT(br_datafile *df, br_light *lp)
{
    df->prims->chunk_write(df, FID_LIGHT, df->prims->struct_size(df, &br_light_F, lp));
    df->prims->struct_write(df, &br_light_F, lp);

    return 0;
}

static int FopRead_LIGHT(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_light *lp;

    /*
     * Allocate and read in light structure
     */
    lp = BrResAllocate(fw.res, sizeof(*lp), BR_MEMORY_LIGHT);
    df->res = lp;
    df->prims->struct_read(df, &br_light_F, lp);
    df->res = NULL;

    /*
     * Leave light on stack
     */
    DfPush(DFST_LIGHT, lp, 1);

    return 0;
}

/**
 ** Camera
 **/
static br_file_enum_member camera_type_FM[] = {
    _ENUM_MEMBER(BR_CAMERA_PARALLEL),
    _ENUM_MEMBER(BR_CAMERA_PERSPECTIVE),
};

static _FILE_ENUM(camera_type);

#define _STRUCT_NAME struct br_camera
static br_file_struct_member br_camera_FM[] = {
    _ENUM_8(type, camera_type_F), _ANGLE(field_of_view), _SCALAR(hither_z), _SCALAR(yon_z), _SCALAR(aspect),
    _ASCIZ(identifier),
};
static _FILE_STRUCT(br_camera);
#undef _STRUCT_NAME

static int FopWrite_CAMERA(br_datafile *df, br_camera *cp)
{
    df->prims->chunk_write(df, FID_CAMERA, df->prims->struct_size(df, &br_camera_F, cp));
    df->prims->struct_write(df, &br_camera_F, cp);

    return 0;
}

static int FopRead_CAMERA(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count)
{
    br_camera *cp;

    /*
     * Allocate and read in camera structure
     */
    cp = BrResAllocate(fw.res, sizeof(*cp), BR_MEMORY_CAMERA);
    df->res = cp;
    df->prims->struct_read(df, &br_camera_F, cp);
    df->res = NULL;

    /*
     * Leave camera on stack
     */
    DfPush(DFST_CAMERA, cp, 1);

    return 0;
}

/**
 **	Public file operations
 **/

/*
 * Allocate memory and read the model from the file - bind and material
 * references to the current loaded set - or default if the named material
 * does not exist.
 */

static br_chunks_table_entry ModelLoadEntries[] = {
    {FID_END, 0, FopRead_END},

    {FID_OLD_MATERIAL_INDEX, 0, FopRead_OLD_MATERIAL_INDEX},
    {FID_OLD_VERTICES, 0, FopRead_OLD_VERTICES},
    {FID_OLD_VERTICES_UV, 0, FopRead_OLD_VERTICES_UV},
    {FID_OLD_FACES, 0, FopRead_OLD_FACES},
    {FID_OLD_MODEL, 0, FopRead_OLD_MODEL},
    {FID_OLD_FACES_1, 1, FopRead_OLD_FACES_1},
    {FID_OLD_MODEL_1, 0, FopRead_OLD_MODEL_1},

    {FID_MODEL, 0, FopRead_MODEL},
    {FID_MATERIAL_INDEX, 1, FopRead_MATERIAL_INDEX},
    {FID_VERTICES, 1, FopRead_VERTICES},
    {FID_VERTEX_UV, 1, FopRead_VERTEX_UV},
    {FID_FACES, 1, FopRead_FACES},
    {FID_FACE_MATERIAL, 0, FopRead_FACE_MATERIAL},
    {FID_PIVOT, 0, FopRead_PIVOT},
};

static br_chunks_table ModelLoadTable = {
    BR_ASIZE(ModelLoadEntries),
    ModelLoadEntries,
};

br_uint_32 BR_PUBLIC_ENTRY BrModelLoadMany(char *filename, br_model **models, br_uint_16 num)
{
    int count;
    int r;
    br_datafile *df;

    df = DfOpen(filename, 0);

    if (df == NULL)
        return 0;

    for (count = 0; count < num;)
    {
        r = DfChunksInterpret(df, &ModelLoadTable);

        if (DfTopType() == DFST_MODEL)
            models[count++] = DfPop(DFST_MODEL, 0);

        if (r == 0)
            break;
    }

    DfClose(df);

    return count;
}

/*
 * Write a complete single model.
 *
 * Does not know about groups - which would be a quicker way of acquiring
 * the material index, but makes this code dependant on the groups code
 * staying the same.
 */

static int BR_CALLBACK PtrCompare(const void *a, const void *b)
{
    return *((char **)b) - *((char **)a);
}

static br_uint_32 BR_CALLBACK WriteModel(br_model *mp, br_datafile *df)
{
    br_material **mindex;
    br_vertex *vp;
    int nmaterials;
    int i;
    int has_uv;

    /*
     * Build material index	- there will never be more materials than faces
     */
    if (mp->faces)
    {
        mindex = BrResAllocate(fw.res, mp->nfaces * sizeof(*mindex), BR_MEMORY_MATERIAL_INDEX);

        /*
         * make table of material pointers
         */
        for (i = 0; i < mp->nfaces; i++)
            mindex[i] = mp->faces[i].material;

        /*
         * sort pointers  (reverse by value - NULL should wind up at end)
         */
        BrQsort(mindex, mp->nfaces, sizeof(*mindex), PtrCompare);

        /*
         * Count how many unique pointers there are and compact
         * them to the base of the table
         */
        for (i = 1, nmaterials = 1; i < mp->nfaces; i++)
            if (mindex[i] != mindex[i - 1])
                mindex[nmaterials++] = mindex[i];

        /*
         * Remove any NULL from end of table
         */
        if (mindex[nmaterials - 1] == NULL)
            nmaterials--;
    }

    /*
     * See if model has any non zero U and V components
     */
    has_uv = 0;
    for (i = 0, vp = mp->vertices; i < mp->nvertices; i++, vp++)
    {
        if ((vp->map.v[U] != BR_SCALAR(0.0)) || (vp->map.v[V] != BR_SCALAR(0.0)))
        {
            has_uv = 1;
            break;
        }
    }

    /*
     * Write out chunks
     */
    FopWrite_MODEL(df, mp);

    if (mp->nvertices)
    {
        FopWrite_VERTICES(df, mp->vertices, mp->nvertices);
        if (has_uv)
            FopWrite_VERTEX_UV(df, mp->vertices, mp->nvertices);
    }

    if (mp->nfaces)
    {
        FopWrite_FACES(df, mp->faces, mp->nfaces);
        if (nmaterials)
        {
            FopWrite_MATERIAL_INDEX(df, mindex, nmaterials);
            FopWrite_FACE_MATERIAL(df, mp->faces, mp->nfaces, mindex, nmaterials);
        }
    }

    FopWrite_END(df);

    BrResFree(mindex);

    return 0;
}

/*
 * Save a set of models to the named file.
 *
 * if models == NULL, then all registered models are saved
 */
br_uint_32 BR_PUBLIC_ENTRY BrModelSaveMany(char *filename, br_model **models, br_uint_16 num)
{
    br_datafile *df;
    int i, m = 0;

    /*
     * Open file and write header
     */
    df = DfOpen(filename, 1);

    if (df == NULL)
        return 0;

    FopWrite_FILE_INFO(df, FILE_TYPE_MODEL);

    if (models)
    {
        for (i = 0; i < num; i++)
            WriteModel(models[i], df);
        m = num;
    }
    else
    {
        BrModelEnum(NULL, (br_model_enum_cbfn *)WriteModel, (void *)df);
        m = BrModelCount(NULL);
    }

    DfClose(df);

    return m;
}

/*
 * Load a hierachy of 1 or more actors and their transforms described in
 * a file
 */
static br_chunks_table_entry ActorLoadEntries[] = {
    {FID_END, 0, FopRead_END},

    {FID_ACTOR, 0, FopRead_ACTOR},
    {FID_ACTOR_MODEL, 0, FopRead_ACTOR_MODEL},
    {FID_ACTOR_TRANSFORM, 0, FopRead_ACTOR_TRANSFORM},
    {FID_ACTOR_MATERIAL, 0, FopRead_ACTOR_MATERIAL},
    {FID_ACTOR_LIGHT, 0, FopRead_ACTOR_LIGHT},
    {FID_ACTOR_CAMERA, 0, FopRead_ACTOR_CAMERA},
    {FID_ACTOR_BOUNDS, 0, FopRead_ACTOR_BOUNDS},
    {FID_ACTOR_CLIP_PLANE, 0, FopRead_ACTOR_CLIP_PLANE},
    {FID_ACTOR_ADD_CHILD, 0, FopRead_ACTOR_ADD_CHILD},

    {FID_TRANSFORM_MATRIX34, 0, FopRead_TRANSFORM},
    {FID_TRANSFORM_MATRIX34_LP, 0, FopRead_TRANSFORM},
    {FID_TRANSFORM_QUAT, 0, FopRead_TRANSFORM},
    {FID_TRANSFORM_EULER, 0, FopRead_TRANSFORM},
    {FID_TRANSFORM_LOOK_UP, 0, FopRead_TRANSFORM},
    {FID_TRANSFORM_TRANSLATION, 0, FopRead_TRANSFORM},
    {FID_TRANSFORM_IDENTITY, 0, FopRead_TRANSFORM},

    {FID_BOUNDS, 0, FopRead_BOUNDS},
    {FID_LIGHT, 0, FopRead_LIGHT},
    {FID_CAMERA, 0, FopRead_CAMERA},
    {FID_PLANE, 0, FopRead_PLANE},
};

static br_chunks_table ActorLoadTable = {
    BR_ASIZE(ActorLoadEntries),
    ActorLoadEntries,
};

br_uint_32 BR_PUBLIC_ENTRY BrActorLoadMany(char *filename, br_actor **actors, br_uint_16 num)
{
    br_datafile *df;
    int count;
    int r;

    df = DfOpen(filename, 0);

    if (df == NULL)
        return 0;

    for (count = 0; count < num;)
    {
        r = DfChunksInterpret(df, &ActorLoadTable);

        if (DfTopType() == DFST_ACTOR)
            actors[count++] = DfPop(DFST_ACTOR, 0);

        if (r == 0)
            break;
    }

    DfClose(df);

    return count;
}

/*
 * Save a hierachy of 1 or more actors and their transforms described in a file
 */
static int WriteActor(br_actor *a, br_datafile *df)
{
    br_actor *ap;

    /*
     * Base actor structure
     */
    FopWrite_ACTOR(df, a);

    /*
     * Transform
     */
    if (a->t.type != BR_TRANSFORM_IDENTITY)
    {
        FopWrite_TRANSFORM(df, &a->t);
        FopWrite_ACTOR_TRANSFORM(df);
    }

    /*
     * Model and material references, if they exist
     */
    if (a->material)
        FopWrite_ACTOR_MATERIAL(df, a->material);

    if (a->model)
        FopWrite_ACTOR_MODEL(df, a->model);

    /*
     * Type specific data
     */
    if (a->type_data)
    {
        switch (a->type)
        {
        case BR_ACTOR_LIGHT:
            FopWrite_LIGHT(df, a->type_data);
            FopWrite_ACTOR_LIGHT(df);
            break;

        case BR_ACTOR_CAMERA:
            FopWrite_CAMERA(df, a->type_data);
            FopWrite_ACTOR_CAMERA(df);
            break;

        case BR_ACTOR_BOUNDS:
            FopWrite_BOUNDS(df, a->type_data);
            FopWrite_ACTOR_BOUNDS(df);
            break;

        case BR_ACTOR_CLIP_PLANE:
            FopWrite_PLANE(df, a->type_data);
            FopWrite_ACTOR_CLIP_PLANE(df);
            break;
        }
    }

    /*
     * Children
     */
    BR_FOR_SIMPLELIST(&a->children, ap)
    {
        WriteActor(ap, df);
        FopWrite_ACTOR_ADD_CHILD(df);
    }

    return 0;
}

/*
 * Save a set of actors to the named file.
 */
br_uint_32 BR_PUBLIC_ENTRY BrActorSaveMany(char *filename, br_actor **actors, br_uint_16 num)
{
    br_datafile *df;
    int i;

    /*
     * Open file and write header
     */
    df = DfOpen(filename, 1);

    if (df == NULL)
        return 0;

    FopWrite_FILE_INFO(df, FILE_TYPE_ACTORS);

    /*
     * Write out all the actors
     */
    for (i = 0; i < num; i++)
    {
        WriteActor(actors[i], df);
        FopWrite_END(df);
    }

    DfClose(df);

    return num;
}

/*
 * Load a group of materials from a file
 */
static br_chunks_table_entry MaterialLoadEntries[] = {
    {FID_END, 0, FopRead_END},
    {FID_MATERIAL, 0, FopRead_MATERIAL},
    {FID_COLOUR_MAP_REF, 0, FopRead_PIXELMAP_REF},
    {FID_INDEX_BLEND_REF, 0, FopRead_PIXELMAP_REF},
    {FID_INDEX_SHADE_REF, 0, FopRead_PIXELMAP_REF},
    {FID_SCREENDOOR_REF, 0, FopRead_PIXELMAP_REF},
};

static br_chunks_table MaterialLoadTable = {
    BR_ASIZE(MaterialLoadEntries),
    MaterialLoadEntries,
};

br_uint_32 BR_PUBLIC_ENTRY BrMaterialLoadMany(char *filename, br_material **materials, br_uint_16 num)
{
    br_datafile *df;
    int count;
    int r;

    df = DfOpen(filename, 0);

    if (df == NULL)
        return 0;

    for (count = 0; count < num;)
    {
        r = DfChunksInterpret(df, &MaterialLoadTable);

        if (DfTopType() == DFST_MATERIAL)
            materials[count++] = DfPop(DFST_MATERIAL, 0);

        if (r == 0)
            break;
    }

    DfClose(df);

    return count;
}

/*
 * Save a set of materials to the named file.
 *
 * if materials == NULL, then all registered materials are saved
 */
static br_uint_32 BR_CALLBACK WriteMaterial(br_material *mp, br_datafile *df)
{
    /*
     * Write base material structure
     */
    FopWrite_MATERIAL(df, mp);

    /*
     * Write out any references to pixelmaps
     */

    if (mp->colour_map)
        FopWrite_PIXELMAP_REF(df, FID_COLOUR_MAP_REF, mp->colour_map);

    if (mp->index_shade)
        FopWrite_PIXELMAP_REF(df, FID_INDEX_SHADE_REF, mp->index_shade);

    if (mp->index_blend)
        FopWrite_PIXELMAP_REF(df, FID_INDEX_BLEND_REF, mp->index_blend);

    if (mp->screendoor)
        FopWrite_PIXELMAP_REF(df, FID_SCREENDOOR_REF, mp->screendoor);

    FopWrite_END(df);

    return 0;
}

br_uint_32 BR_PUBLIC_ENTRY BrMaterialSaveMany(char *filename, br_material **materials, br_uint_16 num)
{
    br_datafile *df;
    int i, count;

    /*
     * Open file and write header
     */
    df = DfOpen(filename, 1);

    if (df == NULL)
        return 0;

    FopWrite_FILE_INFO(df, FILE_TYPE_MATERIAL);

    if (materials)
    {
        for (i = 0; i < num; i++)
            WriteMaterial(materials[i], df);
        count = num;
    }
    else
    {
        BrMaterialEnum(NULL, (br_material_enum_cbfn *)WriteMaterial, df);
        count = BrMaterialCount(NULL);
    }

    DfClose(df);

    return count;
}

/*
 * Load a group of pixelmaps from a file
 */
static br_chunks_table_entry PixelmapLoadEntries[] = {
    {FID_END, 0, FopRead_END},
    {FID_PIXELMAP, 0, FopRead_PIXELMAP},
    {FID_PIXELS, 0, FopRead_PIXELS},
    {FID_ADD_MAP, 0, FopRead_ADD_MAP},
};

static br_chunks_table PixelmapLoadTable = {
    BR_ASIZE(PixelmapLoadEntries),
    PixelmapLoadEntries,
};

br_uint_32 BR_PUBLIC_ENTRY BrPixelmapLoadMany(char *filename, br_pixelmap **pixelmaps, br_uint_16 num)
{
    br_datafile *df;
    int count;
    int r;

    df = DfOpen(filename, 0);

    if (df == NULL)
        return 0;

    for (count = 0; count < num;)
    {
        r = DfChunksInterpret(df, &PixelmapLoadTable);

        if (DfTopType() == DFST_PIXELMAP)
            pixelmaps[count++] = DfPop(DFST_PIXELMAP, 0);

        if (r == 0)
            break;
    }

    DfClose(df);

    return count;
}

/*
 * Save a group of pixelmaps to a file
 */
static int WritePixelmap(br_pixelmap *pp, br_datafile *df)
{
    ASSERT(!(pp->flags & BR_PMF_NO_ACCESS));

    /*
     * Write base pixelmap structure
     */
    FopWrite_PIXELMAP(df, pp);

    /*
     * Write any palette
     */
    if (pp->map)
    {
        WritePixelmap(pp->map, df);
        FopWrite_ADD_MAP(df);
    }

    /*
     * Write pixel data
     */
    FopWrite_PIXELS(df, pp);

    return 0;
}

br_uint_32 BR_PUBLIC_ENTRY BrPixelmapSaveMany(char *filename, br_pixelmap **pixelmaps, br_uint_16 num)
{
    br_datafile *df;
    int i;

    ASSERT(filename != NULL);
    ASSERT(pixelmaps != NULL);

    /*
     * Open file and write header
     */
    df = DfOpen(filename, 1);

    if (df == NULL)
        return 0;

    FopWrite_FILE_INFO(df, FILE_TYPE_PIXELMAP);

    for (i = 0; i < num; i++)
    {
        WritePixelmap(pixelmaps[i], df);
        FopWrite_END(df);
    }

    DfClose(df);

    return num;
}

br_model *BR_PUBLIC_ENTRY BrModelLoad(char *filename)
{
    br_model *ptr;

    return (BrModelLoadMany(filename, &ptr, 1) != 1) ? NULL : ptr;
}

br_uint_32 BR_PUBLIC_ENTRY BrModelSave(char *filename, br_model *ptr)
{
    return BrModelSaveMany(filename, &ptr, 1);
}

br_material *BR_PUBLIC_ENTRY BrMaterialLoad(char *filename)
{
    br_material *ptr;

    return (BrMaterialLoadMany(filename, &ptr, 1) != 1) ? NULL : ptr;
}

br_uint_32 BR_PUBLIC_ENTRY BrMaterialSave(char *filename, br_material *ptr)
{
    return BrMaterialSaveMany(filename, &ptr, 1);
}

br_pixelmap *BR_PUBLIC_ENTRY BrPixelmapLoad(char *filename)
{
    br_pixelmap *ptr;

    return (BrPixelmapLoadMany(filename, &ptr, 1) != 1) ? NULL : ptr;
}

br_uint_32 BR_PUBLIC_ENTRY BrPixelmapSave(char *filename, br_pixelmap *ptr)
{
    return BrPixelmapSaveMany(filename, &ptr, 1);
}

br_actor *BR_PUBLIC_ENTRY BrActorLoad(char *filename)
{
    br_actor *ptr;

    return (BrActorLoadMany(filename, &ptr, 1) != 1) ? NULL : ptr;
}

br_uint_32 BR_PUBLIC_ENTRY BrActorSave(char *filename, br_actor *ptr)
{
    return BrActorSaveMany(filename, &ptr, 1);
}
