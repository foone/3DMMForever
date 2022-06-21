/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: surface.c 1.11 1995/03/29 16:41:24 sam Exp $
 * $Locker:  $
 *
 * Generation of surface properties
 */
#include "fw.h"
#include "shortcut.h"
#include "brassert.h"

static char rscid[] = "$Id: surface.c 1.11 1995/03/29 16:41:24 sam Exp $";

/*
 * Setup surface (lighting mapping etc) prior to a rendering traversal
 */
void SurfacePerScene(br_actor *world, int true_colour)
{
    int i, nm = 0, nv = 0, nc = 0;
    br_light *light;
    br_matrix34 this_to_world, this_to_view, view_to_this;
    br_matrix4 screen_to_view;
    br_matrix4 screen_to_this, tmp4;
    br_active_light *alp;

    /*
     * Set up lighting sub funtions depending on type of output pixels
     */

    if (true_colour)
    {
        fw.fn_table.direct = LightingColour_Dirn;
        fw.fn_table.point = LightingColour_Point;
        fw.fn_table.point_attn = LightingColour_PointAttn;
        fw.fn_table.spot = LightingColour_Spot;
        fw.fn_table.spot_attn = LightingColour_SpotAttn;
    }
    else
    {
        fw.fn_table.direct = LightingIndex_Dirn;
        fw.fn_table.point = LightingIndex_Point;
        fw.fn_table.point_attn = LightingIndex_PointAttn;
        fw.fn_table.spot = LightingIndex_Spot;
        fw.fn_table.spot_attn = LightingIndex_SpotAttn;
    }

    /*
     * Go through active light actors converting into active light
     * instances
     */
    for (i = 0; i < BR_MAX_LIGHTS; i++)
    {
        if (fw.enabled_lights[i] == NULL)
            continue;

        light = fw.enabled_lights[i]->type_data;

        ASSERT(fw.enabled_lights[i]->type == BR_ACTOR_LIGHT);
        ASSERT(light != NULL);
        ASSERT(nm < BR_MAX_LIGHTS);
        ASSERT(nv < BR_MAX_LIGHTS);

        /*
         * Work out light->root transform - ignore light if not part of
         * current hierachy
         */
        if (BrActorToRoot(fw.enabled_lights[i], world, &this_to_world) == 0)
            continue;

        /*
         * Concatenate with world->view
         */
        BrMatrix34Mul(&this_to_view, &this_to_world, &fw.model_to_view);

        /*
         * Build view->light
         */
        BrMatrix34Inverse(&view_to_this, &this_to_view);

        /*
         * Select next active light structure
         */
        if (light->type & BR_LIGHT_VIEW)
            alp = fw.active_lights_view + nv++;
        else
            alp = fw.active_lights_model + nm++;

        /*
         * Generate active data for type of light
         */
        alp->light = light;
        alp->type = light->type & BR_LIGHT_TYPE;

        alp->intensity = BR_RCP(light->attenuation_c);

        switch (alp->type)
        {

        case BR_LIGHT_POINT:
            /*
             * Transform position (0,0,0,1) into view space
             */
            BrVector3CopyMat34Row(&alp->view_position, &this_to_view, 3);

            if (light->attenuation_l == BR_SCALAR(0.0) && light->attenuation_q == BR_SCALAR(0.0))
                alp->light_sub_function = fw.fn_table.point;
            else
                alp->light_sub_function = fw.fn_table.point_attn;
            break;

        case BR_LIGHT_DIRECT:
            /*
             * Transform direction (0,0,1,0) into view space -
             * use T(I(l_to_v)) - or column 2 of view_to_this
             */
            BrVector3CopyMat34Col(&alp->view_direction, &view_to_this, 2);
            alp->light_sub_function = fw.fn_table.direct;

            if (light->type & BR_LIGHT_VIEW)
                /*
                 * Scale direction vector by light intensity for direct lights
                 */
                BrVector3Scale(&alp->direction, &alp->view_direction, alp->intensity);

            break;

        case BR_LIGHT_SPOT:
            /*
             * Transform position and direction into view space
             */
            BrVector3CopyMat34Row(&alp->view_position, &this_to_view, 3);
            BrVector3CopyMat34Col(&alp->view_direction, &view_to_this, 2);

            if (light->attenuation_l == BR_SCALAR(0.0) && light->attenuation_q == BR_SCALAR(0.0))
                alp->light_sub_function = fw.fn_table.spot;
            else
                alp->light_sub_function = fw.fn_table.spot_attn;

            alp->spot_cosine_outer = BR_COS(light->cone_outer);
            alp->spot_cosine_inner = BR_COS(light->cone_inner);

            break;
        }

        /*
         * Copy vectors for view lights
         */
        if (light->type & BR_LIGHT_VIEW)
        {
            alp->position = alp->view_position;
            alp->direction = alp->view_direction;
        }
    }

    fw.nactive_lights_model = nm;
    fw.nactive_lights_view = nv;

    /*
     * Spot special case of 'easy' lighting
     */
    if (fw.nactive_lights_model == 1 && fw.nactive_lights_view == 0 &&
        fw.active_lights_model[0].type == BR_LIGHT_DIRECT)
        fw.light_is_1md = 1;
    else
        fw.light_is_1md = 0;

    /*
     * Set up surface funtions
     */
    if (true_colour)
    {
        fw.fn_table.light = fw.light_is_1md ? LightingColour_1MD : LightingColour;
        fw.fn_table.light_material = LightingColourCopyMaterial;
        fw.fn_table.light_vertex = LightingColourCopyVertex;
        fw.fn_table.face_light = LightingFaceColour;
        fw.fn_table.light_texture = LightingColour_1MDT;
    }
    else
    {
        fw.fn_table.light = fw.light_is_1md ? LightingIndex_1MD : LightingIndex;
        fw.fn_table.light_material = LightingIndexCopyMaterial;
        fw.fn_table.light_vertex = LightingIndexCopyVertex;
        fw.fn_table.face_light = LightingFaceIndex;
        fw.fn_table.light_texture = LightingIndex_1MDT;
    }

    /*
     * Handle any enviroment anchor
     */
    if (fw.enabled_environment)
    {
        if (fw.enabled_environment != world)
        {
            if (!BrActorToRoot(fw.enabled_environment, world, &this_to_world))
                BR_ERROR0("environment is not in world hierachy");
            BrMatrix34Inverse(&fw.model_to_environment, &this_to_world);
        }
        else
        {
            BrMatrix34Identity(&fw.model_to_environment);
        }
    }

    /*
     * Process any clip planes
     */
    nc = 0;

    for (i = 0; i < BR_MAX_CLIP_PLANES; i++)
    {

        if (fw.enabled_clip_planes[i] == NULL)
            continue;

        if (nc == 0)
        {
            /*
             * Invert view->screen
             */
            BrMatrix4Inverse(&screen_to_view, &fw.view_to_screen);
        }

        /*
         * Find clip plane->world, ignore if not in world
         */
        if (BrActorToRoot(fw.enabled_clip_planes[i], world, &this_to_world) == 0)
            continue;

        /*
         * Make plane->view and invert it
         */
        BrMatrix34Mul(&this_to_view, &this_to_world, &fw.model_to_view);
        BrMatrix34Inverse(&view_to_this, &this_to_view);

        /*
         * Make screen->plane
         */
        BrMatrix4Copy34(&tmp4, &view_to_this);

        BrMatrix4Mul(&screen_to_this, &screen_to_view, &tmp4);

        /*
         * Push plane through to screen space
         */
        BrMatrix4TApply(&fw.active_clip_planes[nc].screen_plane, fw.enabled_clip_planes[i]->type_data, &screen_to_this);

        nc++;
    }

    fw.nactive_clip_planes = nc;
}

/*
 * Process active lights before descending into a model
 */
void SurfacePerModel(void)
{
    int i;
    br_active_light *alp;

    alp = fw.active_lights_model;

    for (i = 0; i < fw.nactive_lights_model; i++, alp++)
    {
        switch (alp->type)
        {
        case BR_LIGHT_DIRECT:
            /*
             * Transform light's direction
             */
            BrMatrix34TApplyV(&alp->direction, &alp->view_direction, &fw.model_to_view);
            BrVector3Normalise(&alp->direction, &alp->direction);

            /*
             * Work out a unit half vector
             */
            BrVector3Add(&alp->half, &alp->direction, &fw.eye_m_normalised);
            BrVector3Normalise(&alp->half, &alp->half);

            /*
             * Scale direction vector by light intensity
             */
            BrVector3Scale(&alp->direction, &alp->direction, alp->intensity);
            break;

        case BR_LIGHT_SPOT:
            /*
             * Transform light's direction and position
             */
            BrMatrix34TApplyV(&alp->direction, &alp->view_direction, &fw.model_to_view);
            BrVector3Normalise(&alp->direction, &alp->direction);

            /* FALL THROUGH */

        case BR_LIGHT_POINT:
            /*
             * Transform light's position
             */
            BrMatrix34ApplyP(&alp->position, &alp->view_position, &fw.view_to_model);
            break;
        }
    }

    alp = fw.active_lights_view;

    for (i = 0; i < fw.nactive_lights_view; i++, alp++)
    {

        if (alp->type == BR_LIGHT_DIRECT)
        {

            /*
             * Work out a unit half vector:
             *  eye = (0,0,1)
             *  half = normalise(light_direection + eye)
             */
            alp->half = alp->view_direction;
            alp->half.v[Z] += BR_SCALAR(1.0);
            BrVector3Normalise(&alp->half, &alp->half);
        }
    }
}

/*
 * Dummy functions - in case renderer decides to invoke
 * inappropriate function ptr
 */
void Surface_Null(br_vertex *v, br_scalar *comp)
{
}

void FaceSurface_Null(br_vertex *v, br_scalar *comp)
{
}

/*
 * Process active lights before doing a group of lighting from the same material
 */
void SurfacePerMaterial(br_material *mat)
{
    br_scalar map_width, map_height;

    fw.material = mat;

    UASSERT(mat != NULL);
    UASSERT(mat->prep_flags & MATUF_REGISTERED);

    ASSERT(mat->rptr != NULL);

    /*
     * Catch special case of lit texture mapping
     */
    if (fw.light_is_1md && mat->colour_map &&
        (mat->flags & (BR_MATF_LIGHT | BR_MATF_ENVIRONMENT_I | BR_MATF_ENVIRONMENT_L | BR_MATF_DECAL |
                       BR_MATF_I_FROM_U | BR_MATF_I_FROM_V | BR_MATF_U_FROM_I | BR_MATF_V_FROM_I | BR_MATF_PRELIT)) ==
            BR_MATF_LIGHT)
    {

        fw.surface_fn = fw.fn_table.light_texture;
        fw.face_surface_fn = fw.fn_table.face_light;

        map_width = BrIntToScalar(mat->colour_map->width);
        map_height = BrIntToScalar(mat->colour_map->height);

        fw.map_transform.m[0][0] = BR_MUL(mat->map_transform.m[0][0], map_width);
        fw.map_transform.m[1][0] = BR_MUL(mat->map_transform.m[1][0], map_width);
        fw.map_transform.m[2][0] = BR_MUL(mat->map_transform.m[2][0], map_width);

        fw.map_transform.m[0][1] = BR_MUL(mat->map_transform.m[0][1], map_height);
        fw.map_transform.m[1][1] = BR_MUL(mat->map_transform.m[1][1], map_height);
        fw.map_transform.m[2][1] = BR_MUL(mat->map_transform.m[2][1], map_height);

        fw.index_base = BR_SCALAR(0.5);
        if (mat->index_shade)
            fw.index_range = BrIntToScalar(mat->index_shade->height - 1);
        return;
    }

    /*
     * Select the basic colour functions
     */
    switch (mat->flags & (BR_MATF_LIGHT | BR_MATF_PRELIT))
    {

    case 0:
        fw.surface_fn = fw.fn_table.light_material;
        fw.face_surface_fn = fw.fn_table.face_light;
        break;

    case BR_MATF_LIGHT:

        fw.surface_fn = fw.fn_table.light;
        fw.face_surface_fn = fw.fn_table.face_light;
        break;

    case BR_MATF_PRELIT:
    case BR_MATF_PRELIT | BR_MATF_LIGHT:

        fw.surface_fn = fw.fn_table.light_vertex;
        fw.face_surface_fn = fw.fn_table.face_light;
        break;
    }

    /*
     * Does a mapping function need to be added as well?
     */
    if (mat->colour_map)
    {

        fw.surface_fn_after_map = fw.surface_fn;

        if (mat->flags & BR_MATF_ENVIRONMENT_I)
            /*
             * Environment map (infinite eye)
             */
            fw.surface_fn = MapEnvironmentInfinite2D;
        else if (mat->flags & BR_MATF_ENVIRONMENT_L)
            /*
             * Environment map (local eye)
             */
            fw.surface_fn = MapEnvironmentLocal2D;
        else if (mat->flags & (BR_MATF_LIGHT | BR_MATF_PRELIT))
            /*
             * Normal texture mapping
             */
            fw.surface_fn = MapFromVertex;
        else
            /*
             * Texture, but no lighting
             */
            fw.surface_fn = MapFromVertexOnly;

        /*
         * If decalling - postpone scaling until triangle renderer
         */
        fw.index_base = BR_SCALAR(0.5);
        if (mat->flags & BR_MATF_DECAL)
        {
            fw.index_range = BR_SCALAR(255.0);
        }
        else
        {
            if (mat->index_shade)
                fw.index_range = BrIntToScalar(mat->index_shade->height - 1);
        }

        /*
         * Setup  map_base and map_scale
         */
        map_width = BrIntToScalar(mat->colour_map->width);
        map_height = BrIntToScalar(mat->colour_map->height);

        fw.map_transform.m[0][0] = BR_MUL(mat->map_transform.m[0][0], map_width);
        fw.map_transform.m[1][0] = BR_MUL(mat->map_transform.m[1][0], map_width);
        fw.map_transform.m[2][0] = BR_MUL(mat->map_transform.m[2][0], map_width);

        fw.map_transform.m[0][1] = BR_MUL(mat->map_transform.m[0][1], map_height);
        fw.map_transform.m[1][1] = BR_MUL(mat->map_transform.m[1][1], map_height);
        fw.map_transform.m[2][1] = BR_MUL(mat->map_transform.m[2][1], map_height);
    }
    else
    {
        /*
         * Default base and range
         */
        fw.index_base = BrIntToScalar(mat->index_base) + BR_SCALAR(0.5);
        fw.index_range = BrIntToScalar(mat->index_range);
    }

    /*
     * Does a copy function need to be added?
     */
    if (mat->flags & (BR_MATF_I_FROM_U | BR_MATF_I_FROM_V | BR_MATF_U_FROM_I | BR_MATF_V_FROM_I))
    {

        fw.surface_fn_after_copy = fw.surface_fn;
        fw.surface_fn = CopyComponents;
    }
}

/*
 * Add a light to the set that will illuminate the world
 */
void BR_PUBLIC_ENTRY BrLightEnable(br_actor *l)
{
    int i;

    UASSERT(l != NULL);
    UASSERT(l->type == BR_ACTOR_LIGHT);

    /*
     * Look to see if light is already enabled
     */
    for (i = 0; i < BR_MAX_LIGHTS; i++)
        if (fw.enabled_lights[i] == l)
            return;

    /*
     * Find a blank slot
     */
    for (i = 0; i < BR_MAX_LIGHTS; i++)
    {
        if (fw.enabled_lights[i] == NULL)
        {
            fw.enabled_lights[i] = l;
            return;
        }
    }

    BR_ERROR0("too many enabled lights");
}

/*
 * Remove a light from the set that will illuminate the world
 */
void BR_PUBLIC_ENTRY BrLightDisable(br_actor *l)
{
    int i;

    UASSERT(l != NULL);
    UASSERT(l->type == BR_ACTOR_LIGHT);

    /*
     * Find light in table and remove it
     */
    for (i = 0; i < BR_MAX_LIGHTS; i++)
    {
        if (fw.enabled_lights[i] == l)
        {
            fw.enabled_lights[i] = NULL;
            return;
        }
    }
}

/*
 * Add a clip plane to world
 */
void BR_PUBLIC_ENTRY BrClipPlaneEnable(br_actor *cp)
{
    int i;

    UASSERT(cp != NULL);
    UASSERT(cp->type == BR_ACTOR_CLIP_PLANE);
    UASSERT(cp->type_data != NULL);

    /*
     * Look to see if plane is already enabled
     */
    for (i = 0; i < BR_MAX_CLIP_PLANES; i++)
        if (fw.enabled_clip_planes[i] == cp)
            return;

    /*
     * Find a blank slot
     */
    for (i = 0; i < BR_MAX_CLIP_PLANES; i++)
    {
        if (fw.enabled_clip_planes[i] == NULL)
        {
            fw.enabled_clip_planes[i] = cp;
            return;
        }
    }

    BR_ERROR0("too many enabled clip planes");
}

/*
 * Remove a clip plane
 */
void BR_PUBLIC_ENTRY BrClipPlaneDisable(br_actor *cp)
{
    int i;

    UASSERT(cp != NULL);
    UASSERT(cp->type == BR_ACTOR_CLIP_PLANE);

    /*
     * Find plane in table and remove it
     */
    for (i = 0; i < BR_MAX_CLIP_PLANES; i++)
    {
        if (fw.enabled_clip_planes[i] == cp)
        {
            fw.enabled_clip_planes[i] = NULL;
            return;
        }
    }
}

/*
 * Surface function to copy components around
 */
void BR_SURFACE_CALL CopyComponents(br_vertex *v, br_fvector3 *normal, br_scalar *comp)
{
    /*
     * Call next surface function
     */
    fw.surface_fn_after_copy(v, normal, comp);

    /*
     * Do required component copies
     */
    if (fw.material->flags & BR_MATF_I_FROM_U)
        comp[C_I] = comp[C_U];

    if (fw.material->flags & BR_MATF_I_FROM_V)
        comp[C_I] = comp[C_V];

    if (fw.material->flags & BR_MATF_U_FROM_I)
        comp[C_U] = comp[C_I];

    if (fw.material->flags & BR_MATF_V_FROM_I)
        comp[C_V] = comp[C_I];
}
