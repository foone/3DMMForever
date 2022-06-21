/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: rendsupt.c 1.36 1995/02/22 21:42:33 sam Exp $
 * $Locker:  $
 *
 * Misc. support routines for renderer
 */
#include <string.h>

#include "fw.h"
#include "brassert.h"
#include "datafile.h"

static char rscid[] = "$Id: rendsupt.c 1.36 1995/02/22 21:42:33 sam Exp $";

/*
 * Accumulate the transform between an actor and the root
 *
 * XXX Could accumulate a flag to say that matrix is only rotation
 *     and translation - makes invertion cheaper
 *
 * Returns a flag to indicate if the actor did have world
 * as an ancestor
 */
int BrActorToRoot(br_actor *a, br_actor *world, br_matrix34 *m)
{
    UASSERT(a != NULL);
    UASSERT(world != NULL);
    UASSERT(m != NULL);

    /*
     * Catch stupid case
     */
    if (a == world)
    {
        BrMatrix34Identity(m);
        return 1;
    }

    /*
     * Start with actor's transform
     */
    BrMatrix34Transform(m, &a->t);
    a = a->parent;

    /*
     * Accumulate all the transforms up to the root (ignoring any
     * identity transforms)
     */
    for (; a && a != world; a = a->parent)
        if (a->t.type != BR_TRANSFORM_IDENTITY)
            BrMatrix34PostTransform(m, &a->t);

    return (a == world);
}

/*
 * Invoke a callback for each one of an actors children
 */
br_uint_32 BR_PUBLIC_ENTRY BrActorEnum(br_actor *parent, br_actor_enum_cbfn *callback, void *arg)
{
    br_actor *a;
    br_uint_32 r;

    UASSERT(parent != NULL);
    UASSERT(callback != NULL);

    BR_FOR_SIMPLELIST(&parent->children, a)
    if ((r = callback(a, arg)))
        return r;

    return 0;
}

/*
 * Find all the named actors matching a pattern in the given hierachy
 *
 */
int BR_PUBLIC_ENTRY BrActorSearchMany(br_actor *root, char *pattern, br_actor **actors, int max)
{
    br_actor *a;
    char *sub;
    int n, remaining = max;

    UASSERT(root != NULL);
    UASSERT(actors != NULL);

    /*
     * Take first component of path, and match it against all children
     * of the root actor
     */

    while (*pattern == '/')
        pattern++;

    for (sub = pattern; (*sub != '\0') && (*sub != '/'); sub++)
        ;

    while (*sub == '/')
        sub++;

    BR_FOR_SIMPLELIST(&root->children, a)
    {

        /*
         * Abort if no more space for results
         */
        if (remaining <= 0)
            break;

        if (NamePatternMatch(pattern, a->identifier))
        {
            /*
             * If there is more path
             * 		recurse with remainder of path
             * else
             * 		add actor to accumulated list
             */

            if (*sub != '\0')
            {
                n = BrActorSearchMany(a, sub, actors, remaining);
                actors += n;
                remaining -= n;
            }
            else
            {
                *actors++ = a;
                remaining--;
            }
        }
    }

    return max - remaining;
}

/*
 * Find the first named actor matching a pattern in the given hierachy
 */
br_actor *BR_PUBLIC_ENTRY BrActorSearch(br_actor *root, char *pattern)
{
    br_actor *a;

    UASSERT(root != NULL);

    if (BrActorSearchMany(root, pattern, &a, 1) == 1)
        return a;
    else
        return NULL;
}

/*
 * Recursive function to propgate depth number down a hierachy
 */
static void RenumberActor(br_actor *a, int d)
{
    br_actor *ac;

    a->depth = d;

    BR_FOR_SIMPLELIST(&a->children, ac)
    RenumberActor(ac, d + 1);
}

/*
 * Add an actor to the children of the given parent
 */
br_actor *BR_PUBLIC_ENTRY BrActorAdd(br_actor *parent, br_actor *a)
{
    br_actor *ac;

    UASSERT(a != NULL);
    UASSERT(parent != NULL);
    UASSERT(a->prev == NULL);

    /*
     * Link this actor into sibling list of parent
     */
    BR_SIMPLEADDHEAD(&parent->children, a);
    a->parent = parent;

    /*
     * Update depth for added hierachy
     */
    a->depth = parent->depth + 1;

    BR_FOR_SIMPLELIST(&a->children, ac)
    RenumberActor(ac, a->depth + 1);

    return a;
}

br_actor *BR_PUBLIC_ENTRY BrActorRemove(br_actor *a)
{
    br_actor *ac;

    UASSERT(a != NULL);
    UASSERT(a->prev != NULL);

    BR_SIMPLEREMOVE(a);

    /*
     * Renumber the removed hierachy
     */
    a->depth = 0;

    BR_FOR_SIMPLELIST(&a->children, ac)
    RenumberActor(ac, 1);

    return a;
}

/*
 * Move an actor in the heirachy, but preserve it's apparent world
 * transform by manipulating the transform
 */
void BR_PUBLIC_ENTRY BrActorRelink(br_actor *parent, br_actor *a)
{
}

/*
 * Allocate a new actor structure and return a pointer to it
 *
 * Model actors are allocated from a pool associated with the renderer -
 * this is a very common case and is worth the speedup
 *
 * All other actor types come from the normal heap - and include any
 * type specific data bocks
 *
 */
br_actor *BR_PUBLIC_ENTRY BrActorAllocate(br_uint_8 type, void *type_data)
{
    br_actor *a = NULL;
    br_light *light;
    br_camera *camera;
    br_bounds *bounds;
    br_vector4 *clip_plane;

    UASSERT(type < BR_ACTOR_MAX);

    a = BrResAllocate(fw.res, sizeof(*a), BR_MEMORY_ACTOR);

    /*
     * Initialise common fields
     */
    BrSimpleNewList((struct br_simple_list *)&a->children);
    a->type = type;
    a->depth = 0;
    a->t.type = BR_TRANSFORM_MATRIX34;
    BrMatrix34Identity(&a->t.t.mat);

    if (type_data)
    {

        /*
         * Fill in the type_data pointer
         */
        a->type_data = type_data;
    }
    else
    {
        /*
         * No type data - allocate a default
         */
        switch (type)
        {

        case BR_ACTOR_LIGHT:
            /*
             * Setup to be a directional light source
             */
            light = BrResAllocate(a, sizeof(*light), BR_MEMORY_LIGHT);
            a->type_data = light;

            light->type = BR_LIGHT_DIRECT;
            light->colour = BR_COLOUR_RGB(255, 255, 255);
            light->attenuation_c = BR_SCALAR(1.0);
            light->cone_outer = BR_ANGLE_DEG(15.0);
            light->cone_inner = BR_ANGLE_DEG(10.0);
            break;

        case BR_ACTOR_CAMERA:
            /*
             * Perspective camera, 45 degree FOV
             */
            camera = BrResAllocate(a, sizeof(*camera), BR_MEMORY_CAMERA);
            a->type_data = camera;

            camera->type = BR_CAMERA_PERSPECTIVE;
            camera->field_of_view = BR_ANGLE_DEG(45);
            camera->hither_z = BR_SCALAR(1.0);
            camera->yon_z = BR_SCALAR(10.0);
            camera->aspect = BR_SCALAR(1.0);
            break;

        case BR_ACTOR_BOUNDS:
            /*
             * Allocate a bounds structure
             */
            bounds = BrResAllocate(a, sizeof(*bounds), BR_MEMORY_BOUNDS);
            a->type_data = bounds;
            break;

        case BR_ACTOR_CLIP_PLANE:
            /*
             * Allocate a 4-vector
             */
            clip_plane = BrResAllocate(a, sizeof(*clip_plane), BR_MEMORY_BOUNDS);
            a->type_data = clip_plane;
            break;
        }
    }

    return a;
}

/*
 * Free an actor
 */
static void InternalActorFree(br_actor *a)
{
    /*
     * Release any children
     */
    while (BR_SIMPLEHEAD(&a->children))
        InternalActorFree(BR_SIMPLEREMOVE(a->children));

    /*
     * Release this actor
     */
    BrResFree(a);
}

void BR_PUBLIC_ENTRY BrActorFree(br_actor *a)
{
    UASSERT(a->prev == NULL);

    InternalActorFree(a);
}

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
    int BR_PUBLIC_ENTRY Br##class##AddMany(type **items, int n)                                                        \
    {                                                                                                                  \
        return RegistryAddMany(&fw.reg, items, n);                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    int BR_PUBLIC_ENTRY Br##class##RemoveMany(type **items, int n)                                                     \
    {                                                                                                                  \
        return RegistryRemoveMany(&fw.reg, items, n);                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    int BR_PUBLIC_ENTRY Br##class##FindMany(char *pattern, type **items, int max)                                      \
    {                                                                                                                  \
        return RegistryFindMany(&fw.reg, pattern, items, max);                                                         \
    }                                                                                                                  \
                                                                                                                       \
    int BR_PUBLIC_ENTRY Br##class##Count(char *pattern)                                                                \
    {                                                                                                                  \
        return RegistryCount(&fw.reg, pattern);                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    int BR_PUBLIC_ENTRY Br##class##Enum(char *pattern, int (*callback)(type * item, void *arg), void *arg)             \
    {                                                                                                                  \
        return RegistryEnum(&fw.reg, pattern, callback, arg);                                                          \
    }

#endif

/*
 * Models
 */
br_model *BR_PUBLIC_ENTRY BrModelAdd(br_model *model)
{
    RegistryAdd(&fw.reg_models, model);
    BrModelPrepare(model, BR_MPREP_ALL);
    return model;
}

br_model *BR_PUBLIC_ENTRY BrModelRemove(br_model *model)
{
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

int BR_PUBLIC_ENTRY BrModelAddMany(br_model **items, int n)
{
    int i, r = 0;

    for (i = 0; i < n; i++)
        if (BrModelAdd(*items++) != NULL)
            r++;

    return r;
}

int BR_PUBLIC_ENTRY BrModelRemoveMany(br_model **items, int n)
{
    return RegistryRemoveMany(&fw.reg_models, (void **)items, n);
}

int BR_PUBLIC_ENTRY BrModelFindMany(char *pattern, br_model **items, int max)
{
    return RegistryFindMany(&fw.reg_models, pattern, (void **)items, max);
}

int BR_PUBLIC_ENTRY BrModelCount(char *pattern)
{
    return RegistryCount(&fw.reg_models, pattern);
}

int BR_PUBLIC_ENTRY BrModelEnum(char *pattern, br_model_enum_cbfn *callback, void *arg)
{
    return RegistryEnum(&fw.reg_models, pattern, (br_enum_cbfn *)callback, arg);
}

/*
 * Materials
 */
br_material *BR_PUBLIC_ENTRY BrMaterialAdd(br_material *material)
{
    BrMaterialPrepare(material, 0);
    return RegistryAdd(&fw.reg_materials, material);
}

br_material *BR_PUBLIC_ENTRY BrMaterialRemove(br_material *material)
{
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

int BR_PUBLIC_ENTRY BrMaterialAddMany(br_material **items, int n)
{
    int i, r = 0;

    for (i = 0; i < n; i++)
        if (BrMaterialAdd(*items++) != NULL)
            r++;

    return r;
}

int BR_PUBLIC_ENTRY BrMaterialRemoveMany(br_material **items, int n)
{
    return RegistryRemoveMany(&fw.reg_materials, (void **)items, n);
}

int BR_PUBLIC_ENTRY BrMaterialFindMany(char *pattern, br_material **items, int max)
{
    return RegistryFindMany(&fw.reg_materials, pattern, (void **)items, max);
}

int BR_PUBLIC_ENTRY BrMaterialCount(char *pattern)
{
    return RegistryCount(&fw.reg_materials, pattern);
}

int BR_PUBLIC_ENTRY BrMaterialEnum(char *pattern, br_material_enum_cbfn *callback, void *arg)
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

int BR_PUBLIC_ENTRY BrMapAddMany(br_pixelmap **items, int n)
{
    return RegistryAddMany(&fw.reg_textures, (void **)items, n);
}

int BR_PUBLIC_ENTRY BrMapRemoveMany(br_pixelmap **items, int n)
{
    return RegistryRemoveMany(&fw.reg_textures, (void **)items, n);
}

int BR_PUBLIC_ENTRY BrMapFindMany(char *pattern, br_pixelmap **items, int max)
{
    return RegistryFindMany(&fw.reg_textures, pattern, (void **)items, max);
}

int BR_PUBLIC_ENTRY BrMapCount(char *pattern)
{
    return RegistryCount(&fw.reg_textures, pattern);
}

int BR_PUBLIC_ENTRY BrMapEnum(char *pattern, br_map_enum_cbfn *callback, void *arg)
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

int BR_PUBLIC_ENTRY BrTableAddMany(br_pixelmap **items, int n)
{
    return RegistryAddMany(&fw.reg_tables, (void **)items, n);
}

int BR_PUBLIC_ENTRY BrTableRemoveMany(br_pixelmap **items, int n)
{
    return RegistryRemoveMany(&fw.reg_tables, (void **)items, n);
}

int BR_PUBLIC_ENTRY BrTableFindMany(char *pattern, br_pixelmap **items, int max)
{
    return RegistryFindMany(&fw.reg_tables, pattern, (void **)items, max);
}

int BR_PUBLIC_ENTRY BrTableCount(char *pattern)
{
    return RegistryCount(&fw.reg_tables, pattern);
}

int BR_PUBLIC_ENTRY BrTableEnum(char *pattern, br_table_enum_cbfn *callback, void *arg)
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

int BR_PUBLIC_ENTRY BrResClassAddMany(br_resource_class **items, int n)
{
    int i;

    for (i = 0; i < n; i++)
        BrResClassAdd(*items++);

    return n;
}

int BR_PUBLIC_ENTRY BrResClassRemoveMany(br_resource_class **items, int n)
{
    int i, r;

    for (i = 0, r = 0; i < n; i++)
        if (BrResClassRemove(*items++))
            r++;

    return r;
}

int BR_PUBLIC_ENTRY BrResClassFindMany(char *pattern, br_resource_class **items, int max)
{
    return RegistryFindMany(&fw.reg_resource_classes, pattern, (void **)items, max);
}

int BR_PUBLIC_ENTRY BrResClassCount(char *pattern)
{
    return RegistryCount(&fw.reg_resource_classes, pattern);
}

int BR_PUBLIC_ENTRY BrResClassEnum(char *pattern, br_resclass_enum_cbfn *callback, void *arg)
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
    UASSERT(map_type >= 0 && map_type <= BR_APPLYMAP_CYLINDER);

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

    if (name)
        m->identifier = BrResStrDup(m, name);

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
