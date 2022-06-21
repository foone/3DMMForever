#include <brender.h>
#include <limits.h>
#include "fwproto.h"
#include "zb.h"

br_uint_32 noffset;

static inline void __TriangleRenderZ2(br_boolean use_light, br_boolean use_bump, br_boolean use_transparency,
                                      br_uint_32 bpp)
{
    // TODO(bjrkk): To be translated

outer:
    if (zb.awsl.edge->count == 0)
    {
        return;
    }
    zb.awsl.edge->count--;

    if (use_bump)
    {
        noffset = zb.bump_buffer - zb.texture_buffer;
    }

    zb.pz.currentpix = zb.pz.current;
    if (use_light)
    {
        zb.pi.currentpix = zb.pi.current;
    }

    if (zb.awsl.start < zb.awsl.end)
    {
    }
    else
    {
    }
}

void BR_ASM_CALL TrapezoidRenderPIZ2TA()
{
    __TriangleRenderZ2(BR_FALSE, BR_FALSE, BR_TRUE, 1);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TA_RGB_555()
{
    __TriangleRenderZ2(BR_FALSE, BR_FALSE, BR_TRUE, 2);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TA_RGB_888()
{
    __TriangleRenderZ2(BR_FALSE, BR_FALSE, BR_TRUE, 3);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TAN()
{
    __TriangleRenderZ2(BR_FALSE, BR_TRUE, BR_TRUE, 1);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TAN_RGB_555()
{
    __TriangleRenderZ2(BR_FALSE, BR_TRUE, BR_TRUE, 2);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TAN_RGB_888()
{
    __TriangleRenderZ2(BR_FALSE, BR_TRUE, BR_TRUE, 3);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TIA()
{
    __TriangleRenderZ2(BR_TRUE, BR_FALSE, BR_TRUE, 1);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TIA_RGB_555()
{
    __TriangleRenderZ2(BR_TRUE, BR_FALSE, BR_TRUE, 2);
}

void BR_ASM_CALL TrapezoidRenderPIZ2TIA_RGB_888()
{
    __TriangleRenderZ2(BR_TRUE, BR_FALSE, BR_TRUE, 3);
}