#include "global.h"

void func_080F2358(void *arg0, s32 arg1, s32 arg2) {
    u8 *base = *(u8 **)((u8 *)arg0 + 0x18);
    u32 offset = arg1;
    u32 value = arg2;
    u32 field;
    u32 mask;

    offset <<= 5;
    offset += (u32)base;
    value <<= 18;
    value >>= 18;
    field = *(u16 *)(offset + 8);
    mask = 0xFFFFC000;
    mask &= field;
    mask |= value;
    *(u16 *)(offset + 8) = mask;
}
