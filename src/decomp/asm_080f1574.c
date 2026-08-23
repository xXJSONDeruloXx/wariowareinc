#include "global.h"

extern u8 *D_030068E8;

u32 func_080F1574(u32 arg0) {
    u8 *base;
    u32 offset;

    base = D_030068E8;
    offset = arg0;
    offset <<= 5;
    offset += (u32)base;
    return ((u32)*(u8 *)offset << 0x1F) >> 0x1F;
}
