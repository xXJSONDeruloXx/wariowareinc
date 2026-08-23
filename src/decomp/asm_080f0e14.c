#include "global.h"

extern u8 *D_030068E8;

void func_080F0E14(u32 arg0, u8 arg1, u8 arg2) {
    u8 **table;
    u8 *base;
    u32 offset;
    u8 *base2;

    table = &D_030068E8;
    base = *table;
    offset = arg0;
    offset <<= 5;
    base[offset + 2] = arg1;
    base2 = *table;
    offset += (u32)base2;
    *(u8 *)(offset + 3) = arg2;
}
