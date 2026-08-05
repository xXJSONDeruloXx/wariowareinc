#include "global.h"
#include "types.h"
#include "graphics.h"

void func_0801F188(void) {
    register u8 *r1 asm("r1") = (u8 *)&gGraphicsBuffer;
    register u32 r2 asm("r2") = *(u16 *)r1;
    register u32 r3 asm("r3") = 0x100;
    register u32 r0 asm("r0");
    asm volatile("" : "+r"(r0), "+r"(r1), "+r"(r2), "+r"(r3));
    r0 = r3;
    r3 = 0;
    r0 |= r2;
    *(u16 *)r1 = r0;
    *(u16 *)(r1 + 0x12) = r3;
}
