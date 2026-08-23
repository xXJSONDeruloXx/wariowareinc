#include "global.h"

extern u8 *D_030068E8;

void func_080F0DFC(u32 arg0) {
    u8 *base;
    u32 offset;
    u32 value;
    u32 mask;

    base = D_030068E8;
    offset = arg0;
    offset <<= 5;
    offset += (u32)base;
    value = *(u8 *)offset;
    mask = 2;
    mask = -mask;
    mask &= value;
    *(u8 *)offset = mask;
}
