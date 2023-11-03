#include <brender.h>
#include <brbuiltin.h>
#include "zb.h"

static inline br_int_32 __SafeDiv(br_int_64 a, br_int_32 b)
{
    br_int_32 sign = b ^ (br_uint_32)(a >> 31);

    if (a < 0)
    {
        a = -a;
    }

    if (b < 0)
    {
        b = -b;
    }
    else if (b == 0)
    {
        return 0;
    }

    if ((a >> 32) >= b)
    {
        return 0;
    }

    // NOTE(???): div *must* be safe now or I'll eat my hat... NOT!
    br_uint_32 result = __BrBuiltinDivU64U32(a, b);

    sign >>= 31;
    result ^= sign;
    result -= sign;
    return result;
}

int BR_ASM_CALL SafeFixedMac2Div(int a, int b, int c, int d, int e)
{
    br_int_64 ab = (br_int_64)a * (br_int_64)b;
    br_int_64 cd = (br_int_64)c * (br_int_64)d;
    return __SafeDiv(ab + cd, e);
}

int BR_ASM_CALL SafeFixedMac3Div(int a, int b, int c, int d, int e, int f, int g)
{
    br_int_64 ab = (br_int_64)a * (br_int_64)b;
    br_int_64 cd = (br_int_64)c * (br_int_64)d;
    br_int_64 ef = (br_int_64)e * (br_int_64)f;
    return __SafeDiv(ab + cd + ef, g);
}