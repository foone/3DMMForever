#include <brender.h>
#include <limits.h>
#include <brbuiltin.h>

#include <string.h>

static br_uint_8 const __bit_to_mask_s[] = {
    0b11111111, 0b01111111, 0b00111111, 0b00011111, 0b00001111, 0b00000111, 0b00000011, 0b00000001, 0b00000000,
};

static br_uint_8 const __bit_to_mask_e[] = {
    0b00000000, 0b10000000, 0b11000000, 0b11100000, 0b11110000, 0b11111000, 0b11111100, 0b11111110, 0b11111111,
};

void BR_ASM_CALL _MemPixelSet(char *dest, br_uint_32 bytes, br_uint_32 colour)
{
    br_uint_32 *dword_dest = (br_uint_32 *)dest;

    br_uint_32 const bit0 = bytes * CHAR_BIT;
    br_uint_32 const bit1 = sizeof(*dword_dest) * CHAR_BIT;

    br_uint_32 mask = ((1 << bit0) - 1) << (bit1 - bit0);

    *dword_dest &= ~mask;
    *dword_dest |= colour & mask;
}

br_uint_32 BR_ASM_CALL _MemPixelGet(char *dest, br_uint_32 bytes)
{
    br_uint_32 *dword_dest = (br_uint_32 *)dest;

    br_uint_32 bit0 = bytes * CHAR_BIT;
    br_uint_32 bit1 = sizeof(*dword_dest) * CHAR_BIT;
    br_uint_32 mask = ((1 << bit0) - 1) << (bit1 - bit0);

    br_uint_32 pixel = *dword_dest;
    return pixel & mask;
}

void BR_ASM_CALL _MemCopy_A(char *dest, char *src, br_uint_32 pixels, br_uint_32 bpp)
{
    br_uint_32 stride = pixels * bpp;

    union {
        char *ptr;
        uintptr_t value;
        br_int_32 *u32;
    } __dest = {.ptr = dest}, __src = {.ptr = src};

    while ((__dest.value & 3) != 0)
    {
        *__dest.ptr = *__src.ptr;
        __dest.ptr++;
        __src.ptr++;
    }

    stride /= sizeof(*__src.u32);
    for (br_uint_32 i = 0; i < stride; i += bpp)
    {
        *__dest.u32 = *__src.u32;
        __dest.ptr += bpp;
        __src.ptr += bpp;
    }

    while (((uintptr_t)stride & 3) != 0)
    {
        *__dest.ptr = *__src.ptr;
        __dest.ptr++;
        __src.ptr++;
        stride--;
    }
}

void BR_ASM_CALL _MemFill_A(char *dest, br_uint_32 pixels, br_uint_32 bpp, br_uint_32 colour)
{
    union {
        char *ptr;
        br_uint_8 *u8;
        br_uint_16 *u16;
        br_uint_32 *u32;
    } __dest = {.ptr = dest};

    br_uint_32 mono = (0x01010101 * (colour & 0xFF)) & ((1 << (bpp * CHAR_BIT)) - 1);
    if (bpp == 1 || colour == mono)
    {
        memset(dest, colour, pixels * bpp);
    }
    else if (bpp == 2)
    {
        for (br_size_t i = 0; i < pixels; i++)
        {
            __dest.u16[i] = colour;
        }
    }
    else if (bpp == 3)
    {
        for (br_size_t i = 0; i < pixels; i += 3)
        {
            __dest.u8[i] = colour >> 16;
            __dest.u8[i + 1] = colour >> 8;
            __dest.u8[i + 2] = colour;
        }
    }
    else
    {
        for (br_size_t i = 0; i < pixels; i++)
        {
            __dest.u32[i] = colour;
        }
    }
}

void BR_ASM_CALL _MemRectCopy_A(char *dest, char *src, br_uint_32 pwidth, br_uint_32 pheight, br_int_32 d_stride,
                                br_int_32 s_stride, br_uint_32 bpp)
{
    br_uint_32 const line_diff = pwidth * bpp;

    d_stride -= line_diff;
    s_stride -= line_diff;

    for (br_uint_32 i = 0; i < pheight; i++)
    {
        _MemCopy_A(dest, src, pwidth, bpp);
        dest += d_stride;
        src += s_stride;
    }
}

void BR_ASM_CALL _MemRectFill_A(char *dest, br_uint_32 pwidth, br_uint_32 pheight, br_int_32 d_stride, br_uint_32 bpp,
                                br_uint_32 colour)
{
    br_uint_32 const line_diff = pwidth * bpp;
    d_stride -= line_diff;

    for (br_uint_32 i = 0; i < pheight; i++)
    {
        _MemFill_A(dest, pwidth, bpp, colour);
        dest += d_stride;
    }
}

static inline void __COPY_BITS_CORE_N(char *dest, br_int_32 d_stride, br_uint_8 *src, br_uint_32 s_stride,
                                      br_uint_8 mask_s, br_uint_8 mask_e, br_uint_32 nrows, br_uint_32 bpp,
                                      br_uint_32 colour)
{
    // TODO(bjrkk): Get this one properly sorted out as it is currently broken
    do
    {
        br_int_32 bppt = bpp;
        br_uint_8 src_byte = *src & mask_s;

        while (bppt >= 0)
        {
            char *curr_dest = dest;

            if (bppt == 0)
            {
                src_byte &= mask_e;
            }
            else
            {
                src++;
            }

            for (int x = 0; x < CHAR_BIT; x++)
            {
                if (!__BrBuiltinAddU8Overflow(src_byte, src_byte, &src_byte))
                {
                    continue;
                }
                if (bpp == 1)
                {
                    curr_dest[x * bpp] = (colour & 0xFF);
                }
                else if (bpp == 2)
                {
                    curr_dest[x * bpp + 0] = (colour & 0xFF00) >> 8;
                    curr_dest[x * bpp + 1] = (colour & 0x00FF);
                }
                else if (bpp == 3)
                {
                    curr_dest[x * bpp + 0] = (colour & 0xFF0000) >> 16;
                    curr_dest[x * bpp + 1] = (colour & 0x00FF00) >> 8;
                    curr_dest[x * bpp + 2] = (colour & 0x0000FF);
                }
                else if (bpp == 4)
                {
                    br_uint_32 *dword_dest = (br_uint_32 *)curr_dest;
                    dword_dest[x] = colour;
                }
            }

            src_byte = *src;
            curr_dest += CHAR_BIT * bpp;
            bppt--;
        }

        dest += d_stride;
        src += s_stride;
        nrows--;
    } while (nrows > 0);
}

static inline void __COPY_BITS_CORE_1(char *dest, br_int_32 d_stride, br_uint_8 *src, br_uint_32 s_stride,
                                      br_uint_8 mask_s, br_uint_8 mask_e, br_uint_32 nrows, br_uint_32 bpp,
                                      br_uint_32 colour)
{
    br_uint_8 mask_combined = mask_s & mask_e;

    do
    {
        br_uint_8 src_byte = *src & mask_combined;
        for (int x = 0; x < CHAR_BIT; x++)
        {
            if (!__BrBuiltinAddU8Overflow(src_byte, src_byte, &src_byte))
            {
                continue;
            }
            if (bpp == 1)
            {
                dest[x * bpp] = (colour & 0xFF);
            }
            else if (bpp == 2)
            {
                dest[x * bpp + 0] = (colour & 0xFF00) >> 8;
                dest[x * bpp + 1] = (colour & 0x00FF);
            }
            else if (bpp == 3)
            {
                dest[x * bpp + 0] = (colour & 0xFF0000) >> 16;
                dest[x * bpp + 1] = (colour & 0x00FF00) >> 8;
                dest[x * bpp + 2] = (colour & 0x0000FF);
            }
            else if (bpp == 4)
            {
                br_uint_32 *dword_dest = (br_uint_32 *)dest;
                dword_dest[x] = colour;
            }
        }
        dest += d_stride;
        src += s_stride;
        nrows--;
    } while (nrows > 0);
}

void BR_ASM_CALL _MemCopyBits_A(char *dest, br_int_32 d_stride, br_uint_8 *src, br_uint_32 s_stride,
                                br_uint_32 start_bit, br_uint_32 end_bit, br_uint_32 nrows, br_uint_32 bpp,
                                br_uint_32 colour)
{
    br_uint_8 mask_s = __bit_to_mask_s[start_bit & 7];
    br_uint_8 mask_e = __bit_to_mask_e[end_bit & 7];

    if ((end_bit >> 3) != 0)
    {
        __COPY_BITS_CORE_N(dest, d_stride, src, s_stride, mask_s, mask_e, nrows, bpp, colour);
    }
    else
    {
        __COPY_BITS_CORE_1(dest, d_stride, src, s_stride, mask_s, mask_e, nrows, bpp, colour);
    }
}