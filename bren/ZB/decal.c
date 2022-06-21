/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: decal.c 1.2 1995/03/01 15:49:32 sam Exp $
 * $Locker:  $
 *
 * Decal triangles
 */
#include "zb.h"
#include "shortcut.h"
#include "blockops.h"
#include "brassert.h"

static char rscid[] = "$Id: decal.c 1.2 1995/03/01 15:49:32 sam Exp $";

void BR_ASM_CALL TriangleRenderPIZ2TAD(struct temp_vertex *v0, struct temp_vertex *v1, struct temp_vertex *v2)
{
    /*
     * Render the undercolour
     */
    zb.pi.current = BrIntToScalar(zb.material->index_base);
    TriangleRenderPIZ2(v0, v1, v2);

    /*
     * Render the texture
     */
    TriangleRenderPIZ2TA(v0, v1, v2);
}

void BR_ASM_CALL TriangleRenderPIZ2TIAD(struct temp_vertex *v0, struct temp_vertex *v1, struct temp_vertex *v2)
{
    struct temp_vertex tv[3];
    br_scalar b, r;

    /*
     * Make copies of the temp. vertices
     */
    tv[0] = *v0;
    tv[1] = *v1;
    tv[2] = *v2;

    /*
     * Render the undercolour
     */
    b = BrIntToScalar(zb.material->index_base);
    r = BR_CONST_DIV(BrIntToScalar(zb.material->index_range), 256);

    tv[0].comp[C_I] = b + BR_MUL(r, v0->comp[C_I]);
    tv[1].comp[C_I] = b + BR_MUL(r, v1->comp[C_I]);
    tv[2].comp[C_I] = b + BR_MUL(r, v2->comp[C_I]);

    TriangleRenderPIZ2I(tv + 0, tv + 1, tv + 2);

#if 1

    /*
     * Render the texture
     */
    r = BR_CONST_DIV(BrIntToScalar(zb.material->index_shade->height), 256);

    tv[0].comp[C_I] = BR_MUL(r, v0->comp[C_I]);
    tv[1].comp[C_I] = BR_MUL(r, v1->comp[C_I]);
    tv[2].comp[C_I] = BR_MUL(r, v2->comp[C_I]);

    TriangleRenderPIZ2TIA(tv + 0, tv + 1, tv + 2);
#endif
}
