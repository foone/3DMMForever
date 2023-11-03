/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: actsupt.c 1.9 1995/08/31 16:29:18 sam Exp $
 * $Locker:  $
 *
 * Actor support routines
 */
#include <string.h>

#include "fw.h"
#include "shortcut.h"
#include "brassert.h"
#include "datafile.h"

static char rscid[] = "$Id: actsupt.c 1.9 1995/08/31 16:29:18 sam Exp $";

/*
 * Invoke a callback for each one of an actors children
 */
br_uint_32 BR_PUBLIC_ENTRY BrActorEnum(br_actor *parent, br_actor_enum_cbfn *callback, void *arg)
{
    br_actor *a, *next;
    br_uint_32 r;

    UASSERT(parent != NULL);
    UASSERT(callback != NULL);

    a = parent->children;

    while (a)
    {
        next = a->next;

        if ((r = callback(a, arg)))
            return r;

        a = next;
    }

    return 0;
}

/*
 * Find all the named actors matching a pattern in the given hierachy
 *
 */
br_uint_32 BR_PUBLIC_ENTRY BrActorSearchMany(br_actor *root, char *pattern, br_actor **actors, int max)
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
    a->parent = NULL;

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
    br_matrix34 mat;

    UASSERT(parent != NULL);
    UASSERT(a != NULL);

    /*
     * Catch simple case
     */
    if (a->parent == parent)
        return;

    /*
     * Find the transform we need
     */
    BrActorToActorMatrix34(&mat, a, parent);

    /*
     * Convert matrix into actors transform
     */
    BrMatrix34ToTransform(&a->t, &mat);

    /*
     * Re-attach into the hierachy
     */
    BrActorAdd(parent, BrActorRemove(a));
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
        case BR_ACTOR_BOUNDS_CORRECT:
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
 * Work out a camera 4x4 matrix
 *
 * Return type of transform, one of BR_VTOS_*
 */
int BrCameraToScreenMatrix4(br_matrix4 *mat, br_actor *camera)
{
    br_camera *camera_type;
    br_matrix34 mat34;

    UASSERT(mat != NULL);
    UASSERT(camera != NULL);
    UASSERT(camera->type == BR_ACTOR_CAMERA);
    UASSERT(camera->type_data != NULL);

    camera_type = camera->type_data;

    UASSERT(camera_type->hither_z > S0);
    UASSERT(camera_type->hither_z < camera_type->yon_z);

    switch (camera_type->type)
    {

    case BR_CAMERA_PERSPECTIVE_FOV:

        BrMatrix4Perspective(mat, camera_type->field_of_view, camera_type->aspect, -camera_type->hither_z,
                             -camera_type->yon_z);

        return BR_VTOS_PERSPECTIVE;

    case BR_CAMERA_PERSPECTIVE_WHD:

        return BR_VTOS_PERSPECTIVE;

    case BR_CAMERA_PARALLEL:

        if (camera_type->width == BR_SCALAR(0.0))
            BR_ERROR0("Parallel camera has zero width");

        if (camera_type->height == BR_SCALAR(0.0))
            BR_ERROR0("Parallel camera has zero height");

        if (camera_type->yon_z <= camera_type->hither_z)
            BR_ERROR0("Parallel camera has invalid yon and hither");

        BrMatrix34Scale(&mat34, BR_RCP(camera_type->width), BR_RCP(camera_type->height),
                        BR_RCP(camera_type->yon_z - camera_type->hither_z));
        BrMatrix34PreTranslate(&mat34, S0, S0, camera_type->hither_z);

        BrMatrix4Copy34(mat, &mat34);
        return BR_VTOS_PARALLEL;
    }

    return 0;
}

/*
 * Private routine that gets the COP in model space
 */
void BrVector3EyeInModel(br_vector3 *eye_m)
{
    switch (fw.vtos_type)
    {
    case BR_VTOS_PARALLEL:
        BrVector3CopyMat34Row(&fw.eye_m, &fw.view_to_model, 2);
        break;

    case BR_VTOS_PERSPECTIVE:
        BrVector3CopyMat34Row(&fw.eye_m, &fw.view_to_model, 3);
        break;
#if 0
	case BR_VTOS_4X4: {
		br_matrix4 screen_to_model;
		br_vector4 e;

		BrMatrix4Inverse(&screen_to_model, &model_to_screen);

		eye_m->v[X] = BR_DIV(screen_to_model.m[Z][X],screen_to_model.m[Z][W]);
		eye_m->v[Y] = BR_DIV(screen_to_model.m[Z][Y],screen_to_model.m[Z][W]);
		eye_m->v[Z] = BR_DIV(screen_to_model.m[Z][Z],screen_to_model.m[Z][W]);

		}
		break;
#endif
    }
}

/*
 * Accumulate the transform between one actor an another
 *
 * Returns a transform type that matches the combined result according
 * to the combination rules in transfrm.c
 *
 */
br_uint_8 BR_PUBLIC_ENTRY BrActorToActorMatrix34(br_matrix34 *m, br_actor *a, br_actor *b)
{
    int d, ad, bd;
    br_matrix34 mata, matb, matc;
    br_uint_8 at, bt;

    /*
     * Spot trivial cases
     */
    /*
     * Same actor
     */
    if (a == b)
    {
        BrMatrix34Identity(m);
        return BR_TRANSFORM_IDENTITY;
    }

    /*
     * A is child of B
     */
    if (a->parent == b)
    {
        BrMatrix34Transform(m, &a->t);
        return a->t.type;
    }

    /*
     * B is child of A
     */
    if (b->parent == a)
    {
        BrMatrix34Transform(&matb, &b->t);

        if (IS_LP(b->t.type))
            BrMatrix34LPInverse(m, &matb);
        else
            BrMatrix34Inverse(m, &matb);

        return b->t.type;
    }

    /*
     * Starting at the lowest actor, move up, accumulating transforms
     * until we get to a common ancestor
     */

    at = BR_TRANSFORM_IDENTITY;
    BrMatrix34Identity(&mata);

    bt = BR_TRANSFORM_IDENTITY;
    BrMatrix34Identity(&matb);

    while (a && b && a != b)
    {

        if (a->depth > b->depth)
        {

            if (a->t.type != BR_TRANSFORM_IDENTITY)
            {
                BrMatrix34PostTransform(&mata, &a->t);
                at = COMBINE_TRANSFORMS(at, a->t.type);
            }
            a = a->parent;
        }
        else if (b->depth > a->depth)
        {

            if (b->t.type != BR_TRANSFORM_IDENTITY)
            {
                BrMatrix34PostTransform(&matb, &b->t);
                bt = COMBINE_TRANSFORMS(bt, b->t.type);
            }
            b = b->parent;
        }
        else
        {

            if (a->t.type != BR_TRANSFORM_IDENTITY)
            {
                BrMatrix34PostTransform(&mata, &a->t);
                at = COMBINE_TRANSFORMS(at, a->t.type);
            }
            if (b->t.type != BR_TRANSFORM_IDENTITY)
            {
                BrMatrix34PostTransform(&matb, &b->t);
                bt = COMBINE_TRANSFORMS(bt, b->t.type);
            }
            b = b->parent;
            a = a->parent;
        }
    }

    /*
     * We now have the tranforms from the actors up to a common parent
     * compose A with inverse(B) to get A->B
     */
    if (bt == BR_TRANSFORM_IDENTITY)
    {
        BrMatrix34Copy(m, &mata);
        return at;
    }

    if (at == BR_TRANSFORM_IDENTITY)
    {
        if (IS_LP(bt))
        {
            BrMatrix34LPInverse(m, &matb);
        }
        else
            BrMatrix34Inverse(m, &matb);

        return bt;
    }

    if (IS_LP(bt))
    {
        BrMatrix34LPInverse(&matc, &matb);
    }
    else
        BrMatrix34Inverse(&matc, &matb);

    BrMatrix34Mul(m, &mata, &matc);

    return COMBINE_TRANSFORMS(at, bt);
}

/*
 * Accumulate the transform between an actor and the screen
 *
 */
void BR_PUBLIC_ENTRY BrActorToScreenMatrix4(br_matrix4 *m, br_actor *a, br_actor *camera)
{
    br_matrix34 a2c;

    UASSERT(m != NULL);
    UASSERT(a != NULL);
    UASSERT(camera != NULL);

    /*
     * Build camera to screen
     */
    BrCameraToScreenMatrix4(m, camera);

    /*
     * Only do more work if the actor is not the camera itself
     */
    if (a != camera)
        BrMatrix4Pre34(m, &a2c);
}
/*
 * Transform a bounding box into another bounding box
 *
 * based on "Transforming Axis-Aligned Bounding Boxes" by James Avro -
 * Gems I, page 548
 */
static void BrMatrix34ApplyBounds(br_bounds *d, br_bounds *s, br_matrix34 *m)
{
    int i, j;
    br_scalar a, b;

    /*
     * Start with translation part
     */
    d->min.v[0] = d->max.v[0] = m->m[3][0];
    d->min.v[1] = d->max.v[1] = m->m[3][1];
    d->min.v[2] = d->max.v[2] = m->m[3][2];

    /*
     * Add in extreme values obtained by computing the products
     * of the min and maxes with the elements of the i'th row
     * of the matrix
     */
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            a = BR_MUL(m->m[j][i], s->min.v[j]);
            b = BR_MUL(m->m[j][i], s->max.v[j]);
            if (a < b)
            {
                d->min.v[i] += a;
                d->max.v[i] += b;
            }
            else
            {
                d->min.v[i] += b;
                d->max.v[i] += a;
            }
        }
    }
}

/*
 * Find the bounding box of an actor and all it's children
 *
 *
 */
static void ActorToBounds(br_bounds *dest, br_actor *ap, br_model *model)
{
    br_actor *a;
    br_bounds new;
    br_matrix34 m2v;
    int i;

    /*
     * Propogate default model
     */
    if (ap->model != NULL)
        model = ap->model;

    /*
     * Prepend this transform
     */
    m2v = fw.model_to_view;
    BrMatrix34PreTransform(&fw.model_to_view, &ap->t);

    /*
     * If actor has bounds, transform and compare
     */
    if (ap->type == BR_ACTOR_MODEL)
    {
        BrMatrix34ApplyBounds(&new, &model->bounds, &fw.model_to_view);

        for (i = 0; i < 3; i++)
        {
            if (new.min.v[i] < dest->min.v[i])
                dest->min.v[i] = new.min.v[i];

            if (new.max.v[i] > dest->max.v[i])
                dest->max.v[i] = new.max.v[i];
        }
    }

    /*
     * Recurse into children
     */
    BR_FOR_SIMPLELIST(&ap->children, a)
    ActorToBounds(dest, a, model);

    fw.model_to_view = m2v;
}

br_bounds *BR_PUBLIC_ENTRY BrActorToBounds(br_bounds *b, br_actor *ap)
{
    br_matrix34 m2v = fw.model_to_view;
    br_model *model;
    br_actor *a;

    if (ap->model == NULL)
        model = fw.default_model;
    else
        model = ap->model;

    BrMatrix34Identity(&fw.model_to_view);

    /*
     * If actor has bounds, transform and compare
     */
    if (ap->type == BR_ACTOR_MODEL)
    {
        *b = model->bounds;
    }
    else
    {
        /*
         * Start with empty bounds
         */
        b->min.v[0] = b->min.v[1] = b->min.v[2] = BR_SCALAR_MAX;
        b->max.v[0] = b->max.v[1] = b->max.v[2] = BR_SCALAR_MIN;
    }

    /*
     * Recurse into children
     */
    BR_FOR_SIMPLELIST(&ap->children, a)
    ActorToBounds(b, a, model);

    fw.model_to_view = m2v;
    return b;
}

/*
 * Find the transform that maps a 2 unit cube centred on origin to the given bounding box
 */
br_matrix34 *BR_PUBLIC_ENTRY BrBoundsToMatrix34(br_matrix34 *mat, br_bounds *bounds)
{
    int i;
    br_vector3 tr, sc;

    /*
     * Find the translation
     */
    for (i = 0; i < 3; i++)
        tr.v[i] = (BR_CONST_DIV(bounds->max.v[i], 2) + BR_CONST_DIV(bounds->min.v[i], 2));

    /*
     * Find the scale
     */
    for (i = 0; i < 3; i++)
        if (bounds->max.v[i] != bounds->min.v[i])
            sc.v[i] = BR_CONST_DIV(bounds->max.v[i], 2) - BR_CONST_DIV(bounds->min.v[i], 2);
        else
            sc.v[i] = BR_SCALAR(1.0);

    BrMatrix34Scale(mat, sc.v[0], sc.v[1], sc.v[2]);
    BrMatrix34PostTranslate(mat, tr.v[0], tr.v[1], tr.v[2]);

    return mat;
}
