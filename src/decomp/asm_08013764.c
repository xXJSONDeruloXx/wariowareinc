#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern u32 D_03000E60;

void func_08013764(u32 arg0) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r2 = (u32)&D_03000E60;
    r4 = 0;
    asm volatile("" : "+r"(r2));
    *(u8 *)r2 = r4;
    r1 = 0x3FF;
    r1 &= r0;
    r1 <<= 8;
    r0 = *(u32 *)r2;
    r5 = 0xFFFC00FF;
    r0 &= r5;
    r0 |= r1;
    *(u32 *)r2 = r0;
    r3 = *(u8 *)(r2 + 2);
    r1 = 0x3D;
    r1 = -r1;
    r0 = r1;
    r0 &= r3;
    *(u8 *)(r2 + 2) = r0;
    *(u32 *)(r2 + 4) = r4;
    r0 = 0xFF;
    *(u8 *)(r2 + 8) = r0;
    r0 = *(u32 *)(r2 + 8);
    r0 &= r5;
    *(u32 *)(r2 + 8) = r0;
    r0 = *(u8 *)(r2 + 0xA);
    r1 &= r0;
    *(u8 *)(r2 + 0xA) = r1;
    *(u32 *)(r2 + 0xC) = r4;
}
#endif
