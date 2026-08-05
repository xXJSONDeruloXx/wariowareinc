#include "global.h"
#include "types.h"
#include "graphics.h"

void func_0801AE70(void) {
    register u8 *r2 asm("r2") = (u8 *)&gGraphicsBuffer;
    register u32 r1 asm("r1") = *(u16 *)r2;
    register u32 r0 asm("r0") = 0xE0FF;
    asm volatile("" : "+r"(r0), "+r"(r1), "+r"(r2));
    r0 &= r1;
    *(u16 *)r2 = r0;
}
