#include <brender.h>
#include <string.h>

void BR_ASM_CALL BrBlockFill(void *dest_ptr, int value, int dwords)
{
    br_uint_32 *dword_dest_ptr = (br_uint_32 *)dest_ptr;
    for (int i = 0; i < dwords; i++)
    {
        dword_dest_ptr[i] = value;
    }
}

void BR_ASM_CALL BrBlockCopy(void *dest_ptr, void *src_ptr, int dwords)
{
    br_uint_32 *dword_dest_ptr = (br_uint_32 *)dest_ptr;
    br_uint_32 *dword_src_ptr = (br_uint_32 *)src_ptr;
    for (int i = 0; i < dwords; i++)
    {
        dword_dest_ptr[i] = dword_src_ptr[i];
    }
}

#if BR_HAS_FAR
void BR_ASM_CALL BrFarBlockCopy(void __far *dest_ptr, void *src_ptr, int dwords)
{
    BrBlockCopy(dest_ptr, src_ptr, dwords);
}
#endif