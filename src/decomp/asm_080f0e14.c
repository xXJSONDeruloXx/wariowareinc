#include "global.h"

extern u8 *D_030068E8;

void func_080F0E14(u32 arg0, u8 arg1, u8 arg2) {
    register u8 **table asm("r4") = &D_030068E8;
    register u8 *base asm("r3") = *table;
    register u32 offset asm("r0") = arg0;
    register u8 *base2 asm("r1");
    offset <<= 5;
    base[offset + 2] = arg1;
    base2 = *table;
    offset += (u32)base2;
    *(u8 *)(offset + 3) = arg2;
}
