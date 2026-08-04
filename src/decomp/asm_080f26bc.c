#include "global.h"

void func_080F26BC(u8 *arg0, s32 arg1, u32 arg2) {
    register u8 *r0 asm("r0") = arg0;
    register u8 *r1 asm("r1") = (u8 *)arg1;
    register u32 r2 asm("r2") = arg2;
    register u8 *r3 asm("r3");

    r2 <<= 24;
    r2 >>= 24;
    r3 = *(u8 **)(r0 + 0x18);
    r1 = (u8 *)((u32)r1 << 5);
    r3 = (u8 *)((u32)r1 + (u32)r3);
    r3[0x1D] = r2;
    r0 = *(u8 **)(r0 + 0x18);
    r1 += (u32)r0;
    r1[0x1E] = r2;
}
