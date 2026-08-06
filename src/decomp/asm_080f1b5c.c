#include "global.h"

u32 func_080F1B5C(void *arg0) {
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r0 asm("r0");

    r1 = *(u32 *)((u8 *)arg0 + 0xC);
    r2 = *(u32 *)((u8 *)r1 + 8);
    r2 <<= 11;
    r2 >>= 25;
    r1 = *(u8 *)((u8 *)arg0 + 1);
    r1 <<= 25;
    r1 >>= 25;
    r1 *= r2;
    r0 = *(u8 *)((u8 *)arg0 + 0x1F);
    r0 *= r1;
    r0 >>= 14;
    return r0;
}
