#include "global.h"
#include "types.h"

void func_080C6898(void) {
    register u32 r0 asm("r0") = (u32)&gCurrentSceneVariable;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r3 = *(u32 *)r0;
    r0 = 0xD0;
    r0 <<= 1;
    r1 = r3 + r0;
    r2 = 0;
    r0 = 0;
    *(u16 *)r1 = r0;
    r1 = 0xD1;
    r1 <<= 1;
    r0 = r3 + r1;
    *(u8 *)r0 = r2;
}
