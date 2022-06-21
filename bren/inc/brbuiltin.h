/*
 * NOTE(bjrkk): this is needed for the assembly translation, as there's some
 * things you can't do as easily in C and are rather implemented as compiler
 * built-ins, so this seeks out to remedy this issue in some "portable" way.
 *
 * basically we try to find if we can use a specific builtin, and if we can't,
 * we instead resort to a naive implementation that might either hopefully get
 * optimized out by the compiler or atleast run well enough.
 */

#include <brender.h>
#include <limits.h>

#ifndef _BRBUILTIN_H_
#define _BRBUILTIN_H_

#if defined(__GNUC__)

#if __has_builtin(__builtin_clz)
#define __BR_BUILTIN_CLZ
#endif
#if __has_builtin(__builtin_add_overflow)
#define __BR_BUILTIN_ADD_OVERFLOW
#endif
#if __has_builtin(__builtin_constant_p)
#define __BR_BUILTIN_CONSTANT_P
#endif
#if __has_builtin(__builtin_unreachable)
#define __BR_BUILTIN_UNREACHABLE
#endif

#elif defined(_MSC_VER)

#include <intrin.h>

#if _MSC_VER >= 1400
#define __BR_BITSCANREVERSE
#endif

#if _MSC_VER >= 1920
#define __BR_DIV64
#endif

#define __BR_ASSUME

#endif

#if defined(__BR_BUILTIN_CLZ)

static inline br_uint_32 __BrBuiltinCLZ(br_uint_32 num)
{
    if (num == 0)
    {
        return 0;
    }
    return __builtin_clz(num) ^ (sizeof(num) * CHAR_BIT - 1);
}

#elif defined(__BR_BITSCANREVERSE)

#pragma intrinsic(_BitScanReverse)

static inline br_uint_32 __BrBuiltinCLZ(br_uint_32 num)
{
    unsigned long index = 0;
    if (_BitScanReverse(&index, num) != 0)
    {
        return (sizeof(num) * CHAR_BIT - 1) - index;
    }
    else
    {
        return (sizeof(num) * CHAR_BIT - 1);
    }
}

#else

static inline br_uint_32 __BrBuiltinCLZ(br_uint_32 num)
{
    static br_uint_8 const debrujin_bit_pos[] = {0, 9,  1,  10, 13, 21, 2,  29, 11, 14, 16, 18, 22, 25, 3, 30,
                                                 8, 12, 20, 28, 15, 17, 24, 7,  19, 27, 23, 6,  26, 5,  4, 31};

    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;

    return debrujin_bit_pos[(num * 0x07c4acddu) >> 27];
}

#endif

#if defined(__BR_BUILTIN_ADD_OVERFLOW)

static inline br_boolean __BrBuiltinAddI32Overflow(br_int_32 a, br_int_32 b, br_int_32 *result)
{
    return __builtin_add_overflow(a, b, result);
}

static inline br_boolean __BrBuiltinAddI16Overflow(br_int_16 a, br_int_16 b, br_int_16 *result)
{
    return __builtin_add_overflow(a, b, result);
}

static inline br_boolean __BrBuiltinAddI8Overflow(br_int_8 a, br_int_8 b, br_int_8 *result)
{
    return __builtin_add_overflow(a, b, result);
}

static inline br_boolean __BrBuiltinMulI32Overflow(br_int_32 a, br_int_32 b, br_int_32 *result)
{
    return __builtin_mul_overflow(a, b, result);
}

static inline br_boolean __BrBuiltinMulI16Overflow(br_int_16 a, br_int_16 b, br_int_16 *result)
{
    return __builtin_mul_overflow(a, b, result);
}

static inline br_boolean __BrBuiltinMulI8Overflow(br_int_8 a, br_int_8 b, br_int_8 *result)
{
    return __builtin_mul_overflow(a, b, result);
}

static inline br_boolean __BrBuiltinAddU32Overflow(br_uint_32 a, br_uint_32 b, br_uint_32 *result)
{
    return __builtin_add_overflow(a, b, result);
}

static inline br_boolean __BrBuiltinAddU16Overflow(br_uint_16 a, br_uint_16 b, br_uint_16 *result)
{
    return __builtin_add_overflow(a, b, result);
}

static inline br_boolean __BrBuiltinAddU8Overflow(br_uint_8 a, br_uint_8 b, br_uint_8 *result)
{
    return __builtin_add_overflow(a, b, result);
}

static inline br_boolean __BrBuiltinMulU32Overflow(br_uint_32 a, br_uint_32 b, br_uint_32 *result)
{
    return __builtin_add_overflow(a, b, result);
}

static inline br_boolean __BrBuiltinMulU16Overflow(br_uint_16 a, br_uint_16 b, br_uint_16 *result)
{
    return __builtin_add_overflow(a, b, result);
}

static inline br_boolean __BrBuiltinMulU8Overflow(br_uint_8 a, br_uint_8 b, br_uint_8 *result)
{
    return __builtin_add_overflow(a, b, result);
}

#else

static inline br_boolean __BrBuiltinAddI32Overflow(br_int_32 a, br_int_32 b, br_int_32 *result)
{
    br_int_64 add = (br_int_64)a + (br_int_64)b;
    *result = add;
    return add > INT_MAX || add < INT_MIN;
}

static inline br_boolean __BrBuiltinAddI16Overflow(br_int_16 a, br_int_16 b, br_int_16 *result)
{
    br_int_32 add = (br_int_32)a + (br_int_32)b;
    *result = add;
    return add > SHRT_MAX || add < SHRT_MIN;
}

static inline br_boolean __BrBuiltinAddI8Overflow(br_int_8 a, br_int_8 b, br_int_8 *result)
{
    br_int_16 add = (br_int_16)a + (br_int_16)b;
    *result = add;
    return add > SCHAR_MAX || add < SCHAR_MIN;
}

static inline br_boolean __BrBuiltinMulI32Overflow(br_int_32 a, br_int_32 b, br_int_32 *result)
{
    br_int_64 add = (br_int_64)a * (br_int_64)b;
    *result = add;
    return add > INT_MAX || add < INT_MIN;
}

static inline br_boolean __BrBuiltinMulI16Overflow(br_int_16 a, br_int_16 b, br_int_16 *result)
{
    br_int_32 add = (br_int_32)a * (br_int_32)b;
    *result = add;
    return add > SHRT_MAX || add < SHRT_MIN;
}

static inline br_boolean __BrBuiltinMulI8Overflow(br_int_8 a, br_int_8 b, br_int_8 *result)
{
    br_int_16 add = (br_int_16)a * (br_int_16)b;
    *result = add;
    return add > SCHAR_MAX || add < SCHAR_MIN;
}

static inline br_boolean __BrBuiltinAddU32Overflow(br_uint_32 a, br_uint_32 b, br_uint_32 *result)
{
    br_uint_64 add = (br_uint_64)a + (br_uint_64)b;
    *result = add;
    return add > UINT_MAX;
}

static inline br_boolean __BrBuiltinAddU16Overflow(br_uint_16 a, br_uint_16 b, br_uint_16 *result)
{
    br_uint_32 add = (br_uint_32)a + (br_uint_32)b;
    *result = add;
    return add > USHRT_MAX;
}

static inline br_boolean __BrBuiltinAddU8Overflow(br_uint_8 a, br_uint_8 b, br_uint_8 *result)
{
    br_uint_16 add = (br_uint_16)a + (br_uint_16)b;
    *result = add;
    return add > UCHAR_MAX;
}

static inline br_boolean __BrBuiltinMulU32Overflow(br_uint_32 a, br_uint_32 b, br_uint_32 *result)
{
    br_uint_64 add = (br_uint_64)a * (br_uint_64)b;
    *result = add;
    return add > UINT_MAX;
}

static inline br_boolean __BrBuiltinMulU16Overflow(br_uint_16 a, br_uint_16 b, br_uint_16 *result)
{
    br_uint_32 add = (br_uint_32)a * (br_uint_32)b;
    *result = add;
    return add > USHRT_MAX;
}

static inline br_boolean __BrBuiltinMulU8Overflow(br_uint_8 a, br_uint_8 b, br_uint_8 *result)
{
    br_uint_16 add = (br_uint_16)a * (br_uint_16)b;
    *result = add;
    return add > UCHAR_MAX;
}

#endif

// NOTE(bjrkk): This exists purely because apparently no compiler can
// properly optimize divisions with a 64-bit dividend and 32-bit divisor
// into an idiv r/m32.
#if defined(__BR_BUILTIN_CONSTANT_P) && (defined(__i386__) || defined(__x86_64__))

static inline br_int_32 __BrBuiltinDivI64I32(br_int_64 a, br_int_32 b)
{
    if (__builtin_constant_p(b))
    {
        return a / b;
    }
    br_int_32 lo = a;
    br_int_32 hi = a >> 32;
    __asm__("idivl %2" : "+a"(lo), "+d"(hi) : "rm"(b));
    return lo;
}

static inline br_uint_32 __BrBuiltinDivU64U32(br_uint_64 a, br_uint_32 b)
{
    if (__builtin_constant_p(b))
    {
        return a / b;
    }
    br_uint_32 lo = a;
    br_uint_32 hi = a >> 32;
    __asm__("divl %2" : "+a"(lo), "+d"(hi) : "rm"(b));
    return lo;
}

#elif defined(__BR_DIV64)

static inline br_int_32 __BrBuiltinDivI64I32(br_int_64 a, br_int_32 b)
{
    return _div64(a, b, NULL);
}

static inline br_uint_32 __BrBuiltinDivU64U32(br_uint_64 a, br_uint_32 b)
{
    return _udiv64(a, b, NULL);
}

#else

static inline br_int_32 __BrBuiltinDivI64I32(br_int_64 a, br_int_32 b)
{
    return a / b;
}

static inline br_uint_32 __BrBuiltinDivU64U32(br_uint_64 a, br_uint_32 b)
{
    return a / b;
}

#endif

#if defined(__BR_BUILTIN_UNREACHABLE)
#define __BrBuiltinUnreachable() __builtin_unreachable()
#elif defined(__BR_ASSUME)
#define __BrBuiltinUnreachable() __assume(0)
#else
#define __BrBuiltinUnreachable()
#endif

static inline br_uint_32 __BrBuiltinROR(br_uint_32 num, br_uint_32 count)
{
    return (num >> count | num << (sizeof(num) * CHAR_BIT - count));
}

#endif // _BRBUILTINS_H_