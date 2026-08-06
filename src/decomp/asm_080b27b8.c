#include "global.h"
#include "types.h"

void func_080B27B8(void) {
    register u32 r0 asm("r0") = (u32)&gCurrentSceneVariable;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");

    r2 = *(u32 *)r0;
    r0 = 0xE4;
    r0 <<= 1;
    r1 = r2 + r0;
    r0 = 0;
    *(u16 *)r1 = r0;
    r0 = 0xE5;
    r0 <<= 1;
    r1 = r2 + r0;
    r0 = 1;
    *(u8 *)r1 = r0;
}
