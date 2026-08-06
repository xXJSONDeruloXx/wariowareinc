#include "global.h"
#include "graphics.h"

void func_080186AC(void) {
    register u32 r2 asm("r2") = (u32)&gGraphicsBuffer;
    register u32 r1 asm("r1") = *(u16 *)r2;
    register u32 r0 asm("r0");

    r0 = 0xDFFF;
    r0 &= r1;
    r1 = 0;
    *(u16 *)r2 = r0;
    *(u16 *)(r2 + 0x3C) = r1;
    r0 = r2;
    r0 += 0x40;
    *(u16 *)r0 = r1;
    r0 += 4;
    *(u16 *)r0 = r1;
    r0 += 2;
    *(u16 *)r0 = r1;
}
