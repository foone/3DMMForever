/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: prelight.c 1.1 1995/03/29 16:41:47 sam Exp $
 * $Locker:  $
 *
 * Bits of the lighting model for indexed pixels
 */
#include "fw.h"
#include "shortcut.h"
#include "brassert.h"

static char rscid[] = "$Id: prelight.c 1.1 1995/03/29 16:41:47 sam Exp $";

/*
 * Set the per-vertex i,r,g,b values for the model.
 *
 * This must be called during rendering - eg: as part of a model callback.
 * If you simply want to light a few models, then sandwich the call between
 * ZbSceneRenderBegin() and ZbSceneRenderEnd()
 *
 * If 'a' != NULL, The values are generated as if the model were attached to
 * the actor 'a', otherwise the model will be in the current frame
 */
void BR_PUBLIC_ENTRY BrSceneModelLight(br_model *model, br_material *default_material, br_actor *root, br_actor *a)
{
    int gv, g, v;
    br_vertex *vp;
    br_material *material;
    br_vertex_group *gp = model->vertex_groups;
    br_scalar comp[NUM_COMPONENTS];
    br_matrix34 m2v;

    UASSERT(fw.rendering == 1);

    if (default_material == NULL)
        default_material = fw.default_material;

    /*
     * Save the old model_to_view matrix
     */
    m2v = fw.model_to_view;

    /*
     * Work out transform from actor to world
     */
    if (a != NULL && root != NULL)
        BrActorToRoot(a, root, &fw.model_to_view);

    BrMatrix34Inverse(&fw.view_to_model, &fw.model_to_view);

    v = 0;

    /*
     * Setup lighting
     */
    SurfacePerModel();

    for (g = 0; g < model->nvertex_groups; g++, gp++)
    {

        fw.material = (gp->material ? gp->material : default_material);
        fw.index_base = BR_SCALAR(0.0);
        fw.index_range = BR_SCALAR(255.0);

        for (gv = 0, vp = gp->vertices; gv < gp->nvertices; gv++, v++, vp++)
        {
            /*
             * Call lighting functions for vertex
             */
            LightingIndex(vp, &vp->n, comp);
            LightingColour(vp, &vp->n, comp);

            /*
             * Copy components into vertex
             */
            vp->index = BrScalarToInt(comp[C_I]);
            vp->red = BrScalarToInt(comp[C_R]);
            vp->grn = BrScalarToInt(comp[C_G]);
            vp->blu = BrScalarToInt(comp[C_B]);
        }
    }

    fw.model_to_view = m2v;
}
