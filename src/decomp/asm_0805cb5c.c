#include "global.h"
#include "types.h"
#include "graphics.h"

void func_0805CB5C(void) {
    register u8 *r0 asm("r0") = (u8 *)&gGraphicsBuffer;
    register u32 r1 asm("r1");
    r0 += 0x54;
    asm volatile("" : "+r"(r0));
    r1 = 0x3FF;
    asm volatile("" : "+r"(r1));
    *(u16 *)r0 = r1;
}
