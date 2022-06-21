/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: zbrendr.c 1.36 1995/08/31 16:48:02 sam Exp $
 * $Locker: sam $
 *
 * Given world, a camera, a vieport and a pixmap - perform the rendering
 */

#include "zb.h"
#include "shortcut.h"
#include "brassert.h"

static char rscid[] = "$Id: zbrendr.c 1.36 1995/08/31 16:48:02 sam Exp $";

/*
 * System defaults
 */
extern br_model default_model;
extern br_material default_material;

static void ZbActorRenderOnScreen(br_actor *ap, br_model *model, br_material *material, br_uint_8 style);

/*
 * Table of functions to call for each render style
 */

static void (*ZbRenderStyleCalls[])(br_actor *actor, br_model *model, br_material *material, br_uint_8 style,
                                    int on_screen) = {
    ZbMeshRender,              /* BR_RSTYLE_DEFAULT			*/
    ZbNullRender,              /* BR_RSTYLE_NONE				*/
    ZbMeshRenderPoints,        /* BR_RSTYLE_POINTS				*/
    ZbMeshRenderEdges,         /* BR_RSTYLE_EDGES				*/
    ZbMeshRender,              /* BR_RSTYLE_FACES				*/
    ZbBoundingBoxRenderPoints, /* BR_RSTYLE_BOUNDING_POINTS	*/
    ZbBoundingBoxRenderEdges,  /* BR_RSTYLE_BOUNDING_EDGES		*/
    ZbBoundingBoxRenderFaces,  /* BR_RSTYLE_BOUNDING_FACES		*/
};

/*
 * Renders a model with current settings
 */
void BR_PUBLIC_ENTRY BrZbModelRender(br_actor *actor, br_model *model, br_material *material, br_uint_8 style,
                                     int on_screen, int use_custom)
{
    UASSERT(fw.rendering);
    UASSERT(model != NULL);
    UASSERT(material != NULL);
    UASSERT(actor != NULL);

    UASSERT(model->prep_flags & MODUF_REGISTERED);

    /*
     * If model has custom callback, invoke that
     */
    if (use_custom && (model->flags & BR_MODF_CUSTOM))
        model->custom(actor, model, material, NULL, style, on_screen, &fw.model_to_view, &fw.model_to_screen);
    else
        ZbRenderStyleCalls[style](actor, model, material, style, on_screen);
}

/*
 * Rendering traversal for the given actor
 *
 */
static void ZbActorRender(br_actor *ap, br_model *model, br_material *material, br_uint_8 style)
{
    /*
     * Saved state
     */
    br_matrix4 m_to_s;
    br_matrix34 m_to_v;
    br_matrix34 m_to_e;
    br_material *this_material;
    br_model *this_model;
    br_actor *a;

    int s;

    /*
     * Ignore actors with no children that are not models, and actors with renderstyle = NONE
     */
    if (ap->children == NULL && ap->type != BR_ACTOR_MODEL)
        return;

    if (ap->render_style == BR_RSTYLE_NONE)
        return;

    /*
     * See if this actor overrides default material, model or style
     */
    this_material = ap->material ? ap->material : material;
    this_model = ap->model ? ap->model : model;
    if (ap->render_style != BR_RSTYLE_DEFAULT)
        style = ap->render_style;

    /*
     * Catch special case of identity transforms
     */
    if (ap->t.type == BR_TRANSFORM_IDENTITY)
    {
        /**
         ** Actor has no transform
         **/
        switch (ap->type)
        {

        case BR_ACTOR_MODEL:
            /*
             * This is a model -  see if model's bounding box is on screen
             */
            if ((s = BrOnScreenCheck(&this_model->bounds)) != OSC_REJECT)
                BrZbModelRender(ap, this_model, this_material, style, s, 1);
            break;

        case BR_ACTOR_BOUNDS:
            /*
             * A bounding box - truncate whole tree if rejected
             */
            if (BrOnScreenCheck(ap->type_data) == OSC_REJECT)
                /* DONT PROCESS CHILDREN */
                return;
            break;

        case BR_ACTOR_BOUNDS_CORRECT:
            /*
             * A garuanteed bounding box - test to see if it is on screen
             */
            switch (BrOnScreenCheck(ap->type_data))
            {

            case OSC_ACCEPT:
                /*
                 * Bounding box is completely on screen - process children with special loop
                 */
                BR_FOR_SIMPLELIST(&ap->children, a)
                ZbActorRenderOnScreen(a, this_model, this_material, style);
                /* FALL THROUGH */

            case OSC_REJECT:
                /* DONT PROCESS CHILDREN */
                return;
            }
        }

        /*
         * Recurse for children
         */
        BR_FOR_SIMPLELIST(&ap->children, a)
        ZbActorRender(a, this_model, this_material, style);
    }
    else
    {
        /**
         ** Actor has a transform
         **/

        /*
         * Save the current transforms
         */
        m_to_s = fw.model_to_screen;
        m_to_v = fw.model_to_view;
        m_to_e = fw.model_to_environment;

        /*
         * See if this actor is on the camera path - if so, generate a new transform
         */
        if (ap == fw.camera_path[ap->depth].a)
        {
            BrMatrix34Inverse(&fw.model_to_view, &fw.camera_path[ap->depth].m);

            BrMatrix4Copy(&fw.model_to_screen, &fw.view_to_screen);
            BrMatrix4Pre34(&fw.model_to_screen, &fw.model_to_view);
        }
        else
        {
            BrMatrix4PreTransform(&fw.model_to_screen, &ap->t);
            BrMatrix34PreTransform(&fw.model_to_view, &ap->t);
        }

        if (fw.enabled_environment)
            BrMatrix34PreTransform(&fw.model_to_environment, &ap->t);

        switch (ap->type)
        {

        case BR_ACTOR_MODEL:
            /*
             * This is a model -  see if model's bounding box is on screen
             */
            if ((s = BrOnScreenCheck(&this_model->bounds)) != OSC_REJECT)
                BrZbModelRender(ap, this_model, this_material, style, s, 1);
            break;

        case BR_ACTOR_BOUNDS:
            /*
             * A bounding box - truncate whole tree if rejected
             */
            if (BrOnScreenCheck(ap->type_data) == OSC_REJECT)
            {
                fw.model_to_view = m_to_v;
                fw.model_to_screen = m_to_s;
                fw.model_to_environment = m_to_e;
                return;
            }
            break;

        case BR_ACTOR_BOUNDS_CORRECT:
            /*
             * A garuanteed bounding box - test to see if it is on screen
             */
            switch (BrOnScreenCheck(ap->type_data))
            {

            case OSC_ACCEPT:
                /*
                 * Bounding box is completely on screen - process children with special loop
                 */
                BR_FOR_SIMPLELIST(&ap->children, a)
                ZbActorRenderOnScreen(a, this_model, this_material, style);
                /* FALL THROUGH */

            case OSC_REJECT:
                /*
                 * Don't process children
                 */
                fw.model_to_view = m_to_v;
                fw.model_to_screen = m_to_s;
                fw.model_to_environment = m_to_e;
                return;
            }
        }

        /*
         * Recurse for children
         */
        BR_FOR_SIMPLELIST(&ap->children, a)
        ZbActorRender(a, this_model, this_material, style);

        /*
         * Restore transforms
         */
        fw.model_to_view = m_to_v;
        fw.model_to_screen = m_to_s;
        fw.model_to_environment = m_to_e;
    }
}

/*
 * Rendering traversal for an actor that is completely on screen, along
 * with any children
 */
static void ZbActorRenderOnScreen(br_actor *ap, br_model *model, br_material *material, br_uint_8 style)
{
    /*
     * Saved state
     */
    br_matrix4 m_to_s;
    br_matrix34 m_to_v;
    br_matrix34 m_to_e;
    br_material *this_material;
    br_model *this_model;
    br_actor *a;

    /*
     * Ignore actors with no children that are not models, and actors with renderstyle = NONE
     */
    if (ap->children == NULL && ap->type != BR_ACTOR_MODEL)
        return;

    if (ap->render_style == BR_RSTYLE_NONE)
        return;

    /*
     * See if this actor overrides default material, model or style
     */
    this_material = ap->material ? ap->material : material;
    this_model = ap->model ? ap->model : model;
    if (ap->render_style != BR_RSTYLE_DEFAULT)
        style = ap->render_style;

    /*
     * Catch special case of identity transforms
     */
    if (ap->t.type == BR_TRANSFORM_IDENTITY)
    {
        /*
         * This actor has an no transform
         */
        if (ap->type == BR_ACTOR_MODEL)
            BrZbModelRender(ap, this_model, this_material, style, OSC_ACCEPT, 1);

        BR_FOR_SIMPLELIST(&ap->children, a)
        ZbActorRenderOnScreen(a, this_model, this_material, style);
        return;
    }
    else
    {
        /*
         * Actor has a transform
         */
        m_to_s = fw.model_to_screen;
        m_to_v = fw.model_to_view;
        m_to_e = fw.model_to_environment;

        /*
         * See if this actor is on the camera path - if so, generate a new transform
         */
        if (ap == fw.camera_path[ap->depth].a)
        {
            /*
             * Save model_to_screen and model_to_view
             */
            BrMatrix34Inverse(&fw.model_to_view, &fw.camera_path[ap->depth].m);

            BrMatrix4Copy(&fw.model_to_screen, &fw.view_to_screen);
            BrMatrix4Pre34(&fw.model_to_screen, &fw.model_to_view);
        }
        else
        {
            BrMatrix4PreTransform(&fw.model_to_screen, &ap->t);
            BrMatrix34PreTransform(&fw.model_to_view, &ap->t);
        }

        if (fw.enabled_environment)
            BrMatrix34PreTransform(&fw.model_to_environment, &ap->t);

        if (ap->type == BR_ACTOR_MODEL)
            BrZbModelRender(ap, this_model, this_material, style, OSC_ACCEPT, 1);

        BR_FOR_SIMPLELIST(&ap->children, a)
        ZbActorRenderOnScreen(a, this_model, this_material, style);

        /*
         * Restore transforms
         */
        fw.model_to_view = m_to_v;
        fw.model_to_screen = m_to_s;
        fw.model_to_environment = m_to_e;
        return;
    }
}

/*
 * BrZbSceneRenderBegin()
 *
 * Setup a new scene to be rendered - processes the camera, lights
 * and environment
 */
void BR_PUBLIC_ENTRY BrZbSceneRenderBegin(br_actor *world, br_actor *camera, br_pixelmap *colour_buffer,
                                          br_pixelmap *depth_buffer)
{
    br_camera *camera_type;
    br_matrix34 camera_tfm;
    br_actor *a;
    int i, j;

    UASSERT(zb.type != NULL);
    UASSERT(zb.type->colour_type == colour_buffer->type);
    UASSERT(zb.type->depth_type == depth_buffer->type);

    UASSERT(fw.rendering == 0);

    /*
     * Set flag to say we are rendering
     */
    fw.rendering = 1;

    /*
     * Constant parameters for renderer
     */
    fw.output = colour_buffer;

    if (depth_buffer->row_bytes * zb.type->colour_row_size != colour_buffer->row_bytes * zb.type->depth_row_size)
        BR_ERROR0("Colour and depth strides do not match");

#if 0
	/*
	 * Work out y*row_width table
	 */
	if(zb.row_width != colour_buffer->row_bytes) {
		for(i=0, j=0; i < MAX_OUTPUT_HEIGHT; i++, j+= zb.row_width)
			zb.row_table[i] = j;
	}
#endif

    zb.row_width = colour_buffer->row_bytes;
    zb.depth_row_width = depth_buffer->row_bytes;

    zb.colour_buffer = (br_uint_8 *)colour_buffer->pixels + colour_buffer->base_y * colour_buffer->row_bytes;
    zb.depth_buffer = (br_fixed_ls *)depth_buffer->pixels + depth_buffer->base_y * depth_buffer->row_bytes;

    /*
     * Parameters of viewport (origin is at pixel centre, so add [0.5,0.5])
     */
    fw.vp_ox = BR_SCALAR(colour_buffer->base_x + colour_buffer->width / 2) + BR_SCALAR(0.5);
    fw.vp_width = BR_SCALAR(colour_buffer->width / 2);

    fw.vp_oy = BR_SCALAR(colour_buffer->height / 2) + BR_SCALAR(0.5);
    fw.vp_height = -BR_SCALAR(colour_buffer->height / 2);

    /*
     * Work out View Transform from info. in camera actor
     */
    fw.vtos_type = BrCameraToScreenMatrix4(&fw.view_to_screen, camera);

    /*
     * Collect transforms from camera to root
     *
     * Make a stack of cumulative transforms for each level between
     * the camera and the root - this is so that model->view
     * transforms can use the shortest route, rather than via the root
     */
    for (i = 0; i < MAX_CAMERA_DEPTH; i++)
    {
        fw.camera_path[i].a = NULL;
    }

    i = camera->depth;
    a = camera;
    BrMatrix34Identity(&fw.camera_path[i].m);

#if 0
	if(i > 0) {
		fw.camera_path[i].a = a;
		BrMatrix34Transform(&fw.camera_path[i].m,&a->t);
		a =a->parent;
		i --;
	}
#endif

    for (; i > 0; a = a->parent, i--)
    {
        ASSERT(a != NULL);
        BrMatrix34Transform(&camera_tfm, &a->t);
        BrMatrix34Mul(&fw.camera_path[i - 1].m, &fw.camera_path[i].m, &camera_tfm);
        fw.camera_path[i].a = a;
    }

    if (world != a)
    {
        BR_ERROR0("camera is not in world hierachy");
    }

    /*
     * Make world->view as initial model->view
     */
    BrMatrix34Inverse(&fw.model_to_view, &fw.camera_path[0].m);

    BrMatrix4Copy(&fw.model_to_screen, &fw.view_to_screen);
    BrMatrix4Pre34(&fw.model_to_screen, &fw.model_to_view);

    /*
     * Preprocess active lights, and environment
     */
    SurfacePerScene(world, zb.type->true_colour);
}

/*
 * BrZbSceneRenderAdd()
 *
 * Add a sub-tree of actors to the current rendering
 *
 */
void BR_PUBLIC_ENTRY BrZbSceneRenderAdd(br_actor *tree)
{
    UASSERT(tree);
    UASSERT(fw.rendering);

    /*
     * Walk the provided world and add transformed models to scene
     */
    ZbActorRender(tree, fw.default_model, fw.default_material, BR_RSTYLE_DEFAULT);
}

/*
 * BrZbSceneRenderEnd()
 *
 * Finish rendering a scene - does nothing, but exists in case we
 * ever need a flush of some sort
 */
void BR_PUBLIC_ENTRY BrZbSceneRenderEnd(void)
{
    UASSERT(fw.rendering);
    fw.rendering = 0;
}

/*
 * BrZbSceneRender()
 *
 * Wrapper that sets up, renders and flushes a scene
 *
 */
void BR_PUBLIC_ENTRY BrZbSceneRender(br_actor *world, br_actor *camera, br_pixelmap *colour_buffer,
                                     br_pixelmap *depth_buffer)
{
    BrZbSceneRenderBegin(world, camera, colour_buffer, depth_buffer);
    BrZbSceneRenderAdd(world);
    BrZbSceneRenderEnd();
}

/*
 * Render function for BR_RSTYLE_NONE
 */
void ZbNullRender(br_actor *actor, br_model *model, br_material *material, br_uint_8 style, int on_screen)
{
}

/*
 * Set a callback function for bounding rectangles
 */
br_renderbounds_cbfn *BR_PUBLIC_ENTRY BrZbSetRenderBoundsCallback(br_renderbounds_cbfn *new_cbfn)
{
    br_renderbounds_cbfn *old_cbfn = zb.bounds_call;

    zb.bounds_call = new_cbfn;

    return old_cbfn;
}
