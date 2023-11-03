/*
 * Copyright (c) 1994-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: pick.c 1.15 1995/08/31 16:29:36 sam Exp $
 * $Locker: sam $
 *
 * Pick traversal and testing
 */
#include "fw.h"
#include "shortcut.h"
#include "brassert.h"

static char rscid[] = "$Id: pick.c 1.15 1995/08/31 16:29:36 sam Exp $";

static br_matrix34 pick_model_to_view;

/*
 * Test a bounding box 'b' against the current pick ray '*rp - t . *rd'
 *
 * (NB, the direction of the ray is -ve as compared to normal usage)
 *
 * The active section of the ray is given with t_near and t_far
 *
 * If the active section of the ray intersets the box, then returns
 * TRUE, and stores the new t_near and t_far
 */
static int PickBoundsTestRay(br_bounds *b, br_vector3 *rp, br_vector3 *rd, br_scalar t_near, br_scalar t_far,
                             br_scalar *new_t_near, br_scalar *new_t_far)
{
    int i;
    br_scalar s, t;

    /*
     * Find section of ray that intersects bounding box, if any, by
     * intersecting the half spaces defined by the bounding planes
     * with the ray.
     *
     * The planes tested against are -
     *
     *		eqn			d
     * +X	1, 0, 0,    -max[0]
     * +Y	0, 1, 0,    -max[1]
     * +Z	0, 0, 1,    -max[2]
     * -X  -1, 0, 0,     min[0]
     * -Y	0,-1, 0,     min[1]
     * -Z	0, 0,-1,     min[2]
     *
     * The plane to line intersection is (for a general case)
     *
     * t = -(d + plane_eqn . position)
     *      --------------------------
     *         plane_eqn . direction
     *
     * The actual calculations take into acount the special case of the
     * equations, and that the direction as passed in is -ve.
     *
     */

    /*
     * For each axis, examine the slab made by the planes that are normal to it
     */
    for (i = 0; i < 3; i++)
    {
        /*
         * See which way ray goes relative to axis
         */
        if (rd->v[i] > (2 * BR_SCALAR_EPSILON))
        {
            /*
             * Positive axis towards ray, negative axis away from ray
             */
            s = BR_RCP(rd->v[i]);

            t = BR_MUL(rp->v[i] - b->max.v[i], s);
            if (t > t_near)
                t_near = t;

            t = BR_MUL(rp->v[i] - b->min.v[i], s);
            if (t < t_far)
                t_far = t;
        }
        else if (rd->v[i] < -(2 * BR_SCALAR_EPSILON))
        {
            /*
             * Positive axis away from ray, negative axis towards ray
             */
            s = BR_RCP(rd->v[i]);

            t = BR_MUL(rp->v[i] - b->max.v[i], s);
            if (t < t_far)
                t_far = t;

            t = BR_MUL(rp->v[i] - b->min.v[i], s);
            if (t > t_near)
                t_near = t;
        }
        else
        {
            /*
             * Ray is perp. to axis - check origin vs. both planes
             */
            if (rp->v[i] > b->max.v[i] || rp->v[i] < b->min.v[i])
            {
                /*
                 * Ray is outside planes
                 */
                return 0;
            }
        }
    }

    /*
     * If there is any of the ray left, record the near point
     * and return true
     */
    if (t_near <= t_far)
    {
        *new_t_near = t_near;
        *new_t_far = t_far;
        return 1;
    }
    else
        return 0;
}

/*
 * Traversal function for Picking, tests and actor, and recurse for
 * children
 */
static int ActorPick2D(br_actor *ap, br_model *model, br_material *material, br_pick2d_cbfn *callback, void *arg)
{
    br_actor *a;
    br_model *this_model;
    br_material *this_material;
    br_matrix34 m_to_v;
    br_matrix34 v_to_m;
    br_scalar t_near, t_far;
    int r = 0;

    this_model = ap->model ? ap->model : model;
    this_material = ap->material ? ap->material : material;

    /*
     * Concatenate transform of this actor onto current
     */
    m_to_v = pick_model_to_view;
    BrMatrix34PreTransform(&pick_model_to_view, &ap->t);

    /*
     * Investigate this actor
     */
    switch (ap->type)
    {

    case BR_ACTOR_MODEL:

        /*
         * Work out view to model transform and transform
         * the pick ray.
         *
         * The pick ray is (0,0,0) - t . (0,0,1), so
         * this coresponds to the fourth and third rows
         * of the v_to_m matrix
         */
        BrMatrix34Inverse(&v_to_m, &pick_model_to_view);

        if (PickBoundsTestRay(&this_model->bounds, (br_vector3 *)v_to_m.m[3], (br_vector3 *)v_to_m.m[2], BR_SCALAR(0.0),
                              BR_SCALAR_MAX, &t_near, &t_far))
        {
            br_vector3 dir;
            dir.v[0] = -v_to_m.m[2][0];
            dir.v[1] = -v_to_m.m[2][1];
            dir.v[2] = -v_to_m.m[2][2];
            /*
             * Invoke callback (with correct direction)
             */
            r = callback(ap, this_model, this_material, (br_vector3 *)v_to_m.m[3], &dir, t_near, t_far, arg);
            if (r)
                break;
        }

        if (r)
            break;

        /* FALL THROUGH */
    default:
        BR_FOR_SIMPLELIST(&ap->children, a)
        {
            r = ActorPick2D(a, this_model, this_material, callback, arg);
            if (r)
                break;
        }

        break;

    case BR_ACTOR_BOUNDS:
    case BR_ACTOR_BOUNDS_CORRECT:
        /*
         * Only recurse to children if view ray intersects bounding box
         */
        UASSERT(a->type_data != NULL);

        BrMatrix34Inverse(&v_to_m, &pick_model_to_view);

        if (PickBoundsTestRay(ap->type_data, (br_vector3 *)v_to_m.m[3], (br_vector3 *)v_to_m.m[2], BR_SCALAR(0.0),
                              BR_SCALAR_MAX, &t_near, &t_far))
            BR_FOR_SIMPLELIST(&ap->children, a)
            {
                r = ActorPick2D(a, this_model, this_material, callback, arg);
                if (r)
                    break;
            }

        break;
    }

    /*
     * Restore transform
     */
    pick_model_to_view = m_to_v;

    return r;
}

/*
 * Pick objects from a given scene by casting a ray through the given
 * viewport pixel attached to the given camera.
 *
 * A callback will be invoked for each actor whose bounds intersect
 * this ray.
 *
 * The callback is given the ray point & direction in the actor's
 * local space, and the 't' values of the intersections of the ray
 * with the bounding box.
 *
 * If the callback returns a non-zero value, traversal stops, and the
 * value is returned from BrScenePick2D
 *
 * A general purpose 'void *' argument is passed down to the callback
 */
int BR_PUBLIC_ENTRY BrScenePick2D(br_actor *world, br_actor *camera, br_pixelmap *viewport, int pick_x, int pick_y,
                                  br_pick2d_cbfn *callback, void *arg)
{
    br_matrix34 camera_tfm;
    br_scalar scale;
    br_camera *camera_data;

    UASSERT(callback != NULL);

    UASSERT(camera->type == BR_ACTOR_CAMERA);
    UASSERT(camera->type_data != NULL);

    camera_data = camera->type_data;

    /*
     * Build view->world transform
     */
    BrActorToRoot(camera, world, &camera_tfm);
    BrMatrix34Inverse(&pick_model_to_view, &camera_tfm);

    /*
     * Build view to screen transform s.t. ray is along -ve Z axis
     * and starts at the origin
     *
     */
    switch (camera_data->type)
    {

    case BR_CAMERA_PERSPECTIVE_FOV:

        scale = BR_DIV(BR_COS((br_angle)(camera_data->field_of_view / 2)),
                       BR_SIN((br_angle)(camera_data->field_of_view / 2)));

        BrMatrix34PostScale(&pick_model_to_view, BR_DIV(scale, camera_data->aspect), scale, BR_SCALAR(1.0));

        BrMatrix34PostShearZ(&pick_model_to_view, BR_DIV(BrIntToScalar(2 * pick_x), BrIntToScalar(viewport->width)),
                             BR_DIV(BrIntToScalar(-2 * pick_y), BrIntToScalar(viewport->height)));
        break;

    case BR_CAMERA_PARALLEL:
        BrMatrix34PostTranslate(&pick_model_to_view, S0, S0, camera_data->hither_z);
        BrMatrix34PostScale(&pick_model_to_view, BR_RCP(camera_data->width), BR_RCP(camera_data->height),
                            BR_RCP(camera_data->yon_z - camera_data->hither_z));

        BrMatrix34PostTranslate(&pick_model_to_view, BR_DIV(BrIntToScalar(-2 * pick_x), BrIntToScalar(viewport->width)),
                                BR_DIV(BrIntToScalar(2 * pick_y), BrIntToScalar(viewport->height)), S0);

        break;
    }
    /*
     * Traverse world
     */
    return ActorPick2D(world, fw.default_model, fw.default_material, callback, arg);
}

/*
 * Work out whether two bounding boxes into different coordinate frames
 * interset
 */
#define MAKE_EQN(axis, sign, distance)                                                                                 \
    {                                                                                                                  \
        eqn.v[0] = sign p_to_m.m[0][axis];                                                                             \
        eqn.v[1] = sign p_to_m.m[1][axis];                                                                             \
        eqn.v[2] = sign p_to_m.m[2][axis];                                                                             \
        eqn.v[3] = sign p_to_m.m[3][axis] distance;                                                                    \
    }

#define TEST_NOT_IN                                                                                                    \
    (BR_MAC3(eqn.v[X], ((eqn.v[X] > 0) ? (bounds->min.v[X]) : (bounds->max.v[X])), eqn.v[Y],                           \
             ((eqn.v[Y] > 0) ? (bounds->min.v[Y]) : (bounds->max.v[Y])), eqn.v[Z],                                     \
             ((eqn.v[Z] > 0) ? (bounds->min.v[Z]) : (bounds->max.v[Z]))) < eqn.v[W])

#define TEST_OUT                                                                                                       \
    (BR_MAC3(eqn.v[X], ((eqn.v[X] > 0) ? (bounds->max.v[X]) : (bounds->min.v[X])), eqn.v[Y],                           \
             ((eqn.v[Y] > 0) ? (bounds->max.v[Y]) : (bounds->min.v[Y])), eqn.v[Z],                                     \
             ((eqn.v[Z] > 0) ? (bounds->max.v[Z]) : (bounds->min.v[Z]))) < eqn.v[W])

static int PickBoundsTestBox(br_bounds *model_bounds, br_bounds *bounds, br_matrix34 *m_to_p)
{
    br_matrix34 p_to_m;
    br_vector4 eqn;
    int i;

    BrMatrix34Inverse(&p_to_m, m_to_p);

    /*
     * For each plane of the model box, check to see if the pick box
     * is completely outside it
     */
    for (i = 0; i < 3; i++)
    {
        /*
         * Min X
         */
        MAKE_EQN(i, -, +model_bounds->min.v[i]);

        if (TEST_OUT)
            return 0;

        /*
         * Max X
         */
        MAKE_EQN(i, +, -model_bounds->max.v[i]);

        if (TEST_OUT)
            return 0;
    }

    return 1;
}

/*
 * Recursive function for 3D picking within a hierachy
 */
static int ActorPick3D(br_actor *ap, br_model *model, br_material *material, br_bounds *bounds,
                       br_pick3d_cbfn *callback, void *arg)
{
    br_actor *a;
    br_model *this_model;
    br_material *this_material;
    br_matrix34 m_to_v;
    int r = 0;

    this_model = ap->model ? ap->model : model;
    this_material = ap->material ? ap->material : material;

    /*
     * Concatenate transform of this actor onto current
     */
    m_to_v = pick_model_to_view;
    BrMatrix34PreTransform(&pick_model_to_view, &ap->t);

    switch (ap->type)
    {

    case BR_ACTOR_MODEL:
        if (PickBoundsTestBox(&this_model->bounds, bounds, &pick_model_to_view))
        {
            /*
             * Invoke callback
             */
            r = callback(ap, this_model, this_material, &m_to_v, bounds, arg);
        }

        /* FALL THROUGH */
    default:
        BR_FOR_SIMPLELIST(&ap->children, a)
        {
            r = ActorPick3D(a, this_model, this_material, bounds, callback, arg);
            if (r)
                break;
        }

        break;

    case BR_ACTOR_BOUNDS:
    case BR_ACTOR_BOUNDS_CORRECT:
        /*
         * Only recurse into bounds if pick_bounds intersects
         */
        UASSERT(a->type_data != NULL);

        if (PickBoundsTestBox(ap->type_data, bounds, &pick_model_to_view))
        {
            BR_FOR_SIMPLELIST(&ap->children, a)
            {
                r = ActorPick3D(a, this_model, this_material, bounds, callback, arg);
                if (r)
                    break;
            }
        }
        break;
    }

    /*
     * Restore transform
     */
    pick_model_to_view = m_to_v;

    return r;
}

/*
 * Find any actors whose bounding boxes intersect a 3D
 * pick box
 */
int BR_PUBLIC_ENTRY BrScenePick3D(br_actor *world, br_actor *actor, br_bounds *bounds, br_pick3d_cbfn *callback,
                                  void *arg)
{
    br_matrix34 mat;

    UASSERT(world != NULL);
    UASSERT(actor != NULL);
    UASSERT(bounds != NULL);
    UASSERT(callback != NULL);

    /*
     * Work out transform from world to base actor
     */
    BrActorToRoot(actor, world, &mat);
    BrMatrix34Inverse(&pick_model_to_view, &mat);

    /*
     * Traverse the world
     */
    return ActorPick3D(world, fw.default_model, fw.default_material, bounds, callback, arg);
}

/*
 * Intersect a models's faces with a ray segment
 *
 * XXX Possible options -
 *
 *	All faces
 *	Nearest face
 *	Nearest vertex of hit face
 *  U,V parameters of hit point
 *
 */
int BR_PUBLIC_ENTRY BrModelPick2D(br_model *model, br_material *material, br_vector3 *ray_pos, br_vector3 *ray_dir,
                                  br_scalar t_near, br_scalar t_far, br_modelpick2d_cbfn *callback, void *arg)
{
    br_face *fp;
    int f, axis_m, axis_0, axis_1;
    br_scalar t, d, n;
    br_vector3 p;
    br_scalar u0, u1, u2, v0, v1, v2;
    br_scalar alpha, beta;
    br_vector2 map;
    int v, e, r;
    br_material *this_material;

    t_near -= BR_SCALAR(0.001);
    t_far += BR_SCALAR(0.001);

    /*
     * For each face in model...
     */
    for (f = 0, fp = model->faces; f < model->nfaces; f++, fp++)
    {
        /*
         * Work out face's material
         */
        this_material = fp->material ? fp->material : material;

        /*
         * Intersect ray with plane of polygon
         */
        d = BrFVector3Dot(&fp->n, ray_dir);

        /*
         * If ray is parallel to face, ignore it
         */
        if (BR_ABS(d) < 2 * BR_SCALAR_EPSILON)
            continue;

        /*
         * If d is +ve, ray goes in same dirn as normal - ignore face if single sided
         */
        if (!(this_material->flags & (BR_MATF_TWO_SIDED | BR_MATF_ALWAYS_VISIBLE)) && d > S0)
            continue;

        t = -BR_DIV(BrFVector3Dot(&fp->n, ray_pos) - fp->d, d);

        /*
         * Ignore face if intersection is outside ray segment
         */
        if (t < t_near || t > t_far)
            continue;

        /*
         * Work out intersection point
         */
        BrVector3Scale(&p, ray_dir, t);
        BrVector3Add(&p, &p, ray_pos);

        /*
         * Work out dominant axis of normal
         */
        axis_m = 0;
        if (BR_ABS(fp->n.v[1]) > BR_ABS(fp->n.v[0]))
            axis_m = 1;

        if (BR_ABS(fp->n.v[2]) > BR_ABS(fp->n.v[axis_m]))
            axis_m = 2;

        switch (axis_m)
        {
        case 0:
            axis_0 = 1;
            axis_1 = 2;
            break;
        case 1:
            axis_0 = 0;
            axis_1 = 2;
            break;
        case 2:
            axis_0 = 0;
            axis_1 = 1;
            break;
        }

        /*
         * Work out point in polygon (Gems I, pg 392)
         */
        u0 = model->vertices[fp->vertices[0]].p.v[axis_0];
        v0 = model->vertices[fp->vertices[0]].p.v[axis_1];

        u1 = model->vertices[fp->vertices[1]].p.v[axis_0] - u0;
        v1 = model->vertices[fp->vertices[1]].p.v[axis_1] - v0;

        u2 = model->vertices[fp->vertices[2]].p.v[axis_0] - u0;
        v2 = model->vertices[fp->vertices[2]].p.v[axis_1] - v0;

        u0 = p.v[axis_0] - u0;
        v0 = p.v[axis_1] - v0;

        if (BR_ABS(u1) <= (2 * BR_SCALAR_EPSILON))
        {

            if (BR_ABS(u0) > BR_ABS(u2))
                continue;

            if (u2 == S0)
                continue;

            beta = BR_DIV(u0, u2);

            if (beta >= S0 && beta <= S1)
                alpha = BR_DIV(BR_SUB(v0, BR_MUL(beta, v2)), v1);
            else
                continue;
        }
        else
        {
            n = BR_MAC2(v0, u1, -u0, v1);
            d = BR_MAC2(v2, u1, -u2, v1);

            if (BR_ABS(n) > BR_ABS(d))
                continue;

            if (d == S0)
                continue;

            beta = BR_DIV(n, d);
            if (beta >= S0 && beta <= S1)
                alpha = BR_DIV(BR_SUB(u0, BR_MUL(beta, u2)), u1);
            else
                continue;
        }

        if (alpha < S0 || (alpha + beta) > S1)
            continue;

        /*
         * Point is in polygon
         */

        /*
         * Interpolate u,v
         */
        BrVector2Scale(&map, &model->vertices[fp->vertices[1]].map, alpha);
        BrVector2AccumulateScale(&map, &model->vertices[fp->vertices[2]].map, beta);
        BrVector2AccumulateScale(&map, &model->vertices[fp->vertices[0]].map, (S1 - (alpha + beta)));

        /*
         * Find `nearest' vertex and edge - partition triangle
         * along the bisectors of each corner
         */
        v = 0;
        e = 1;

        if (alpha > beta)
        {
            /*
             * Edge 2 vs. Edge 1
             */
            if (alpha < (S1 - BR_CONST_MUL(beta, 2)))
                e = 2;
            /*
             * Vertex 2 vs. Edge 0
             */
            if (alpha > (BR_SCALAR(0.5) - BR_CONST_DIV(beta, 2)))
                v = 2;
        }
        else
        {
            /*
             * Edge 0 vs. Edge 1
             */
            if (alpha < (BR_SCALAR(0.5) - BR_CONST_DIV(beta, 2)))
                e = 0;

            /*
             * Vertex 1 vs. Edge 0
             */
            if (alpha > (S1 - BR_CONST_MUL(beta, 2)))
                v = 1;
        }

        /*
         * Invoke callback
         */
        r = callback(model, this_material, ray_pos, ray_dir, t, f, e, v, &p, &map, arg);

        if (r)
            return r;
    }

    return 0;
}
