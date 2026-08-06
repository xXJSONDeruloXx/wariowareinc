#include "global.h"
#include "types.h"

void func_08062488(void) {
    register u32 r0 asm("r0") = (u32)&gCurrentSceneVariable;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");

    r1 = *(u32 *)r0;
    r0 = 0xBD4;
    r1 += r0;
    r2 = *(u8 *)r1;
    r0 = 2;
    r0 = -r0;
    r0 &= r2;
    *(u8 *)r1 = r0;
}
