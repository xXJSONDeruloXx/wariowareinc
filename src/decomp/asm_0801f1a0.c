#include "global.h"
#include "types.h"
#include "graphics.h"

void func_0801F1A0(void) {
    register u8 *r1 asm("r1") = (u8 *)&gGraphicsBuffer;
    register u32 r2 asm("r2") = *(u16 *)r1;
    register u32 r0 asm("r0") = 0xFEFF;
    asm volatile("" : "+r"(r0), "+r"(r1), "+r"(r2));
    r0 &= r2;
    *(u16 *)r1 = r0;
    r0 = 1;
    *(u16 *)(r1 + 0x12) = r0;
}
