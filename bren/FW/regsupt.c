/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: regsupt.c 1.4 1995/03/01 15:26:21 sam Exp $
 * $Locker:  $
 *
 * Public registry functions
 */
#include <string.h>

#include "fw.h"
#include "brassert.h"
#include "datafile.h"

static char rs[] = "$Id: regsupt.c 1.4 1995/03/01 15:26:21 sam Exp $";

/**
 ** Specific versions of registry calls for each class
 **/

#if 0
/*
 * Macro to define all the calls except BrXXXAdd()
 */
#define REG_CALLS(class, type, reg)                                                                                    \
                                                                                                                       \
    type *BR_PUBLIC_ENTRY Br##class##Remove(type *##lname##)                                                           \
    {                                                                                                                  \
        return RegistryRemove(&fw.reg, ##lname##);                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    type *BR_PUBLIC_ENTRY Br##class##Find(char *pattern)                                                               \
    {                                                                                                                  \
        return RegistryFind(&fw.reg, pattern);                                                                         \
    }                                                                                                                  \
                                                                                                                       \
    br_uint_32 BR_PUBLIC_ENTRY Br##class##AddMany(type **items, int n)                                                 \
    {                                                                                                                  \
        return RegistryAddMany(&fw.reg, items, n);                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    br_uint_32 BR_PUBLIC_ENTRY Br##class##RemoveMany(type **items, int n)                                              \
    {                                                                                                                  \
        return RegistryRemoveMany(&fw.reg, items, n);                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    br_uint_32 BR_PUBLIC_ENTRY Br##class##FindMany(char *pattern, type **items, int max)                               \
    {                                                                                                                  \
        return RegistryFindMany(&fw.reg, pattern, items, max);                                                         \
    }                                                                                                                  \
                                                                                                                       \
    br_uint_32 BR_PUBLIC_ENTRY Br##class##Count(char *pattern)                                                         \
    {                                                                                                                  \
        return RegistryCount(&fw.reg, pattern);                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    br_uint_32 BR_PUBLIC_ENTRY Br##class##Enum(char *pattern, int (*callback)(type * item, void *arg), void *arg)      \
    {                                                                                                                  \
        return RegistryEnum(&fw.reg, pattern, callback, arg);                                                          \
    }

#endif

/*
 * Models
 */
br_model *BR_PUBLIC_ENTRY BrModelAdd(br_model *model)
{
    /*
     * Check that model was allocated
     */
    UASSERT(BrResCheck(model, 1));
    UASSERT(BrResClass(model) == BR_MEMORY_MODEL);

    RegistryAdd(&fw.reg_models, model);
    BrModelUpdate(model, BR_MODU_ALL);

    model->prep_flags |= MODUF_REGISTERED;

    return model;
}

br_model *BR_PUBLIC_ENTRY BrModelRemove(br_model *model)
{
    model->prep_flags &= ~MODUF_REGISTERED;

    return RegistryRemove(&fw.reg_models, model);
}

br_model *BR_PUBLIC_ENTRY BrModelFind(char *pattern)
{
    return RegistryFind(&fw.reg_models, pattern);
}

br_model_find_cbfn *BR_PUBLIC_ENTRY BrModelFindHook(br_model_find_cbfn *hook)
{
    br_model_find_cbfn *old = (br_model_find_cbfn *)fw.reg_models.find_failed_hook;
    fw.reg_models.find_failed_hook = (br_find_failed_cbfn *)hook;

    return old;
}

br_uint_32 BR_PUBLIC_ENTRY BrModelAddMany(br_model **items, int n)
{
    int i, r = 0;

    for (i = 0; i < n; i++)
        if (BrModelAdd(*items++) != NULL)
            r++;

    return r;
}

br_uint_32 BR_PUBLIC_ENTRY BrModelRemoveMany(br_model **items, int n)
{
    int i, r = 0;

    for (i = 0; i < n; i++)
        if (BrModelRemove(*items++) != NULL)
            r++;

    return r;
}

br_uint_32 BR_PUBLIC_ENTRY BrModelFindMany(char *pattern, br_model **items, int max)
{
    return RegistryFindMany(&fw.reg_models, pattern, (void **)items, max);
}

br_uint_32 BR_PUBLIC_ENTRY BrModelCount(char *pattern)
{
    return RegistryCount(&fw.reg_models, pattern);
}

br_uint_32 BR_PUBLIC_ENTRY BrModelEnum(char *pattern, br_model_enum_cbfn *callback, void *arg)
{
    return RegistryEnum(&fw.reg_models, pattern, (br_enum_cbfn *)callback, arg);
}

/*
 * Materials
 */
br_material *BR_PUBLIC_ENTRY BrMaterialAdd(br_material *material)
{
    /*
     * Check that material was allocated
     */
    UASSERT(BrResCheck(material, 1));
    UASSERT(BrResClass(material) == BR_MEMORY_MATERIAL);

    BrMaterialUpdate(material, BR_MATU_ALL);

    material->prep_flags |= MODUF_REGISTERED;

    return RegistryAdd(&fw.reg_materials, material);
}

br_material *BR_PUBLIC_ENTRY BrMaterialRemove(br_material *material)
{
    material->prep_flags &= ~MODUF_REGISTERED;

    return RegistryRemove(&fw.reg_materials, material);
}

br_material *BR_PUBLIC_ENTRY BrMaterialFind(char *pattern)
{
    return RegistryFind(&fw.reg_materials, pattern);
}

br_material_find_cbfn *BR_PUBLIC_ENTRY BrMaterialFindHook(br_material_find_cbfn *hook)
{
    br_material_find_cbfn *old = (br_material_find_cbfn *)fw.reg_materials.find_failed_hook;
    fw.reg_materials.find_failed_hook = (br_find_failed_cbfn *)hook;

    return old;
}

br_uint_32 BR_PUBLIC_ENTRY BrMaterialAddMany(br_material **items, int n)
{
    int i, r = 0;

    for (i = 0; i < n; i++)
        if (BrMaterialAdd(*items++) != NULL)
            r++;

    return r;
}

br_uint_32 BR_PUBLIC_ENTRY BrMaterialRemoveMany(br_material **items, int n)
{
    int i, r = 0;

    for (i = 0; i < n; i++)
        if (BrMaterialRemove(*items++) != NULL)
            r++;

    return r;
}

br_uint_32 BR_PUBLIC_ENTRY BrMaterialFindMany(char *pattern, br_material **items, int max)
{
    return RegistryFindMany(&fw.reg_materials, pattern, (void **)items, max);
}

br_uint_32 BR_PUBLIC_ENTRY BrMaterialCount(char *pattern)
{
    return RegistryCount(&fw.reg_materials, pattern);
}

br_uint_32 BR_PUBLIC_ENTRY BrMaterialEnum(char *pattern, br_material_enum_cbfn *callback, void *arg)
{
    return RegistryEnum(&fw.reg_materials, pattern, (br_enum_cbfn *)callback, arg);
}

/*
 * Textures
 */
br_pixelmap *BR_PUBLIC_ENTRY BrMapAdd(br_pixelmap *pixelmap)
{
    return RegistryAdd(&fw.reg_textures, pixelmap);
}

br_pixelmap *BR_PUBLIC_ENTRY BrMapRemove(br_pixelmap *pixelmap)
{
    return RegistryRemove(&fw.reg_textures, pixelmap);
}

br_pixelmap *BR_PUBLIC_ENTRY BrMapFind(char *pattern)
{
    return RegistryFind(&fw.reg_textures, pattern);
}

br_map_find_cbfn *BR_PUBLIC_ENTRY BrMapFindHook(br_map_find_cbfn *hook)
{
    br_map_find_cbfn *old = (br_map_find_cbfn *)fw.reg_textures.find_failed_hook;
    fw.reg_textures.find_failed_hook = (br_find_failed_cbfn *)hook;

    return old;
}

br_uint_32 BR_PUBLIC_ENTRY BrMapAddMany(br_pixelmap **items, int n)
{
    return RegistryAddMany(&fw.reg_textures, (void **)items, n);
}

br_uint_32 BR_PUBLIC_ENTRY BrMapRemoveMany(br_pixelmap **items, int n)
{
    return RegistryRemoveMany(&fw.reg_textures, (void **)items, n);
}

br_uint_32 BR_PUBLIC_ENTRY BrMapFindMany(char *pattern, br_pixelmap **items, int max)
{
    return RegistryFindMany(&fw.reg_textures, pattern, (void **)items, max);
}

br_uint_32 BR_PUBLIC_ENTRY BrMapCount(char *pattern)
{
    return RegistryCount(&fw.reg_textures, pattern);
}

br_uint_32 BR_PUBLIC_ENTRY BrMapEnum(char *pattern, br_map_enum_cbfn *callback, void *arg)
{
    return RegistryEnum(&fw.reg_textures, pattern, (br_enum_cbfn *)callback, arg);
}

/*
 * Tables
 */
br_pixelmap *BR_PUBLIC_ENTRY BrTableAdd(br_pixelmap *pixelmap)
{
    return RegistryAdd(&fw.reg_tables, pixelmap);
}

br_pixelmap *BR_PUBLIC_ENTRY BrTableRemove(br_pixelmap *pixelmap)
{
    return RegistryRemove(&fw.reg_tables, pixelmap);
}

br_pixelmap *BR_PUBLIC_ENTRY BrTableFind(char *pattern)
{
    return RegistryFind(&fw.reg_tables, pattern);
}

br_table_find_cbfn *BR_PUBLIC_ENTRY BrTableFindHook(br_table_find_cbfn *hook)
{
    br_table_find_cbfn *old = (br_table_find_cbfn *)fw.reg_tables.find_failed_hook;
    fw.reg_tables.find_failed_hook = (br_find_failed_cbfn *)hook;

    return old;
}

br_uint_32 BR_PUBLIC_ENTRY BrTableAddMany(br_pixelmap **items, int n)
{
    return RegistryAddMany(&fw.reg_tables, (void **)items, n);
}

br_uint_32 BR_PUBLIC_ENTRY BrTableRemoveMany(br_pixelmap **items, int n)
{
    return RegistryRemoveMany(&fw.reg_tables, (void **)items, n);
}

br_uint_32 BR_PUBLIC_ENTRY BrTableFindMany(char *pattern, br_pixelmap **items, int max)
{
    return RegistryFindMany(&fw.reg_tables, pattern, (void **)items, max);
}

br_uint_32 BR_PUBLIC_ENTRY BrTableCount(char *pattern)
{
    return RegistryCount(&fw.reg_tables, pattern);
}

br_uint_32 BR_PUBLIC_ENTRY BrTableEnum(char *pattern, br_table_enum_cbfn *callback, void *arg)
{

    return RegistryEnum(&fw.reg_tables, pattern, (br_enum_cbfn *)callback, arg);
}

/*
 * Resource Classes
 */
br_resource_class *BR_PUBLIC_ENTRY BrResClassAdd(br_resource_class *rclass)
{
    br_resource_class *r;

    UASSERT(rclass != NULL);

    /*
     * The registry resource is initally faked
     */
    UASSERT(rclass->res_class == BR_MEMORY_REGISTRY || rclass->res_class == BR_MEMORY_ANCHOR ||
            fw.resource_class_index[rclass->res_class] == NULL);

    r = RegistryAdd(&fw.reg_resource_classes, rclass);

    if (r != NULL)
        fw.resource_class_index[rclass->res_class] = r;

    return r;
    r;
}

br_resource_class *BR_PUBLIC_ENTRY BrResClassRemove(br_resource_class *rclass)
{
    br_resource_class *r;

    UASSERT(rclass != NULL);
    UASSERT(fw.resource_class_index[rclass->res_class] != NULL);

    r = RegistryRemove(&fw.reg_resource_classes, rclass);

    if (r != NULL)
        fw.resource_class_index[rclass->res_class] = NULL;

    return r;
}

br_resource_class *BR_PUBLIC_ENTRY BrResClassFind(char *pattern)
{
    return RegistryFind(&fw.reg_resource_classes, pattern);
}

br_resclass_find_cbfn *BR_PUBLIC_ENTRY BrResClassFindHook(br_resclass_find_cbfn *hook)
{
    br_resclass_find_cbfn *old = (br_resclass_find_cbfn *)fw.reg_resource_classes.find_failed_hook;

    fw.reg_resource_classes.find_failed_hook = (br_find_failed_cbfn *)hook;

    return old;
}

br_uint_32 BR_PUBLIC_ENTRY BrResClassAddMany(br_resource_class **items, int n)
{
    int i;

    for (i = 0; i < n; i++)
        BrResClassAdd(*items++);

    return n;
}

br_uint_32 BR_PUBLIC_ENTRY BrResClassRemoveMany(br_resource_class **items, int n)
{
    int i, r;

    for (i = 0, r = 0; i < n; i++)
        if (BrResClassRemove(*items++))
            r++;

    return r;
}

br_uint_32 BR_PUBLIC_ENTRY BrResClassFindMany(char *pattern, br_resource_class **items, int max)
{
    return RegistryFindMany(&fw.reg_resource_classes, pattern, (void **)items, max);
}

br_uint_32 BR_PUBLIC_ENTRY BrResClassCount(char *pattern)
{
    return RegistryCount(&fw.reg_resource_classes, pattern);
}

br_uint_32 BR_PUBLIC_ENTRY BrResClassEnum(char *pattern, br_resclass_enum_cbfn *callback, void *arg)
{
    return RegistryEnum(&fw.reg_resource_classes, pattern, (br_enum_cbfn *)callback, arg);
}

/*
 * Generate U and V values for a model's vertices.
 *
 * The vertices are optionally transformed by a 3x4 matrix
 *
 * The u,v values are then assigned according to one of the
 * following schemes -
 *
 * Planar mapping:
 * 	u = (x+1)/2
 *  v = (y+1)/2
 *
 * Spherical mapping:
 * 	u = atan2(-z,x)/2*pi
 *  v = 1-atan2(sqrt(x*x+z*z),y)/pi
 *
 * Cylindrical mapping:
 * 	u = atan2(-z,x)/2*pi
 *  v = (y+1)/2
 *
 * Disc mapping:
 * 	u = atan2(-z,x)/2*pi
 *  v = sqrt(x*x+z*z)
 */
void BR_PUBLIC_ENTRY BrModelApplyMap(br_model *model, int map_type, br_matrix34 *xform)
{
    int v;
    br_vertex *vp;
    br_vector3 mv;
    br_matrix34 default_xform;
    br_scalar d;

    UASSERT(model != NULL);
    UASSERT(model->vertices != NULL);
    UASSERT(map_type >= 0 && map_type <= BR_APPLYMAP_NONE);

    /*
     * If no default provided, use identity
     */
    if (xform == NULL)
    {
        BrMatrix34Identity(&default_xform);
        xform = &default_xform;
    }

    /*
     * Set mapping for each vertex
     */
    for (v = 0, vp = model->vertices; v < model->nvertices; v++, vp++)
    {
        BrMatrix34ApplyP(&mv, &vp->p, xform);

        switch (map_type)
        {
        case BR_APPLYMAP_NONE:
            /*
             * No mapping:
             * 	u = 0
             * 	v = 0
             */
            vp->map.v[0] = BR_SCALAR(0.0);
            vp->map.v[1] = BR_SCALAR(0.0);
            break;

        case BR_APPLYMAP_PLANE:
            /*
             * Planar mapping:
             * 	u = (x+1)/2
             *  v = (y+1)/2
             */
            vp->map.v[0] = BR_MUL(mv.v[0] + BR_SCALAR(1.0), BrFractionToScalar(BR_FRACTION(0.5)));

            vp->map.v[1] = BR_MUL(mv.v[1] + BR_SCALAR(1.0), BrFractionToScalar(BR_FRACTION(0.5)));
            break;

        case BR_APPLYMAP_SPHERE:
            /*
             * Spherical mapping:
             * 	u = atan2(-z,x)/2*pi
             *  v = 1-atan2(sqrt(x*x+z*z),y)/pi
             */
            vp->map.v[0] = BR_DIV(BrAngleToDegree(BR_ATAN2(-mv.v[2], mv.v[0])), BR_SCALAR(360.0));

            d = BR_LENGTH2(mv.v[0], mv.v[2]);

            vp->map.v[1] = BR_SCALAR(1.0) - BR_DIV(BrAngleToDegree(BR_ATAN2(d, mv.v[1])), BR_SCALAR(180.0));

            break;

        case BR_APPLYMAP_CYLINDER:
            /*
             * Cylindrical mapping:
             * 	u = atan2(-z,x)/2*pi
             *  v = (y+1)/2
             */
            vp->map.v[0] = BR_DIV(BrAngleToDegree(BR_ATAN2(-mv.v[2], mv.v[0])), BR_SCALAR(360.0));

            vp->map.v[1] = BR_MUL(mv.v[1] + BR_SCALAR(1.0), BrFractionToScalar(BR_FRACTION(0.5)));
            break;

        case BR_APPLYMAP_DISC:
            /*
             * Disc mapping:
             * 	u = atan2(-y,x)/2*pi
             *  v = sqrt(x*x+y*y)
             */
            vp->map.v[0] = BR_DIV(BrAngleToDegree(BR_ATAN2(-mv.v[1], mv.v[0])), BR_SCALAR(360.0));

            vp->map.v[1] = BR_LENGTH2(mv.v[0], mv.v[1]);
            break;
        }
    }
}

/*
 * Work out a transform such that it can be fed to BrModelApplyMap
 * and have the mapping type fit the bounds of the model exactly
 *
 * The two axis along which the mapping can be applied are provided -
 *
 *	BR_FITMAP_PLUS_X,
 *	BR_FITMAP_PLUS_Y,
 *	BR_FITMAP_PLUS_Z,
 *	BR_FITMAP_MINUS_X,
 *	BR_FITMAP_MINUS_Y,
 *	BR_FITMAP_MINUS_Z,
 *
 * Returns the supplied pointer to 'transform'
 */

br_matrix34 *BR_PUBLIC_ENTRY BrModelFitMap(br_model *model, int axis_0, int axis_1, br_matrix34 *transform)
{
    br_vector3 axis_3;
    br_vector3 tr;
    br_vector3 sc;
    int i;

    static br_vector3 axis_vectors[] = {
        BR_VECTOR3(1, 0, 0),  /* +X */
        BR_VECTOR3(0, 1, 0),  /* +Y */
        BR_VECTOR3(0, 0, 1),  /* +Z */
        BR_VECTOR3(-1, 0, 0), /* -X */
        BR_VECTOR3(0, -1, 0), /* -Y */
        BR_VECTOR3(0, 0, -1), /* -Z */
    };

    UASSERT(model != NULL);
    UASSERT(transform != NULL);
    UASSERT(axis_0 >= BR_FITMAP_PLUS_X && axis_0 <= BR_FITMAP_MINUS_Z);
    UASSERT(axis_1 >= BR_FITMAP_PLUS_X && axis_1 <= BR_FITMAP_MINUS_Z);

    /*
     * Start the mapping transform by filling in the two user
     * supplied axes (as columns), and then using a cross product to generate
     * the third column.
     */
    BrMatrix34Identity(transform);

    BrVector3Cross(&axis_3, axis_vectors + axis_0, axis_vectors + axis_1);

    for (i = 0; i < 3; i++)
    {
        transform->m[i][0] = axis_vectors[axis_0].v[i];
        transform->m[i][1] = axis_vectors[axis_1].v[i];
        transform->m[i][2] = axis_3.v[i];
    }

    /*
     * Find the translation to align the object with the mapping
     */
    for (i = 0; i < 3; i++)
        tr.v[i] = -(BR_CONST_DIV(model->bounds.max.v[i], 2) + BR_CONST_DIV(model->bounds.min.v[i], 2));

    /*
     * Find the scale to fit object to the mapping
     */
    for (i = 0; i < 3; i++)
        if (model->bounds.max.v[i] != model->bounds.min.v[i])
            sc.v[i] = BR_RCP(BR_CONST_DIV(model->bounds.max.v[i], 2) - BR_CONST_DIV(model->bounds.min.v[i], 2));
        else
            sc.v[i] = BR_SCALAR(1.0);

    BrMatrix34PreScale(transform, sc.v[0], sc.v[1], sc.v[2]);
    BrMatrix34PreTranslate(transform, tr.v[0], tr.v[1], tr.v[2]);

    return transform;
}

/*
 * Free a model
 */
void BR_PUBLIC_ENTRY BrModelFree(br_model *m)
{
    UASSERT(m != NULL);

    BrResFree(m);
}

/*
 * Allocate a model of a given size
 */
br_model *BR_PUBLIC_ENTRY BrModelAllocate(char *name, int nvertices, int nfaces)
{
    br_model *m;

    UASSERT(nvertices >= 0);
    UASSERT(nfaces >= 0);

    m = BrResAllocate(fw.res, sizeof(*m), BR_MEMORY_MODEL);

    m->nvertices = nvertices;
    m->nfaces = nfaces;

    if (name)
        m->identifier = BrResStrDup(m, name);

    if (nvertices)
        m->vertices = BrResAllocate(m, nvertices * sizeof(*m->vertices), BR_MEMORY_VERTICES);

    if (nfaces)
        m->faces = BrResAllocate(m, nfaces * sizeof(*m->faces), BR_MEMORY_FACES);

    return m;
}

/*
 * Allocate a material
 */
br_material *BR_PUBLIC_ENTRY BrMaterialAllocate(char *name)
{
    br_material *m;

    m = BrResAllocate(fw.res, sizeof(*m), BR_MEMORY_MATERIAL);

    *m = *fw.default_material;

    if (name)
        m->identifier = BrResStrDup(m, name);
    else
        m->identifier = NULL;

    return m;
}

/*
 * Free a material
 */
void BR_PUBLIC_ENTRY BrMaterialFree(br_material *m)
{
    UASSERT(m != NULL);

    BrResFree(m);
}
