#include "global.h"
#include "types.h"

void func_080D2768(void) {
    register u32 r1 asm("r1") = (u32)&gCurrentSceneVariable;
    register u32 r0 asm("r0");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r0 = *(u32 *)r1;
    r2 = 0xE6;
    r2 <<= 2;
    r0 += r2;
    r2 = 0;
    *(u8 *)r0 = r2;
    r0 = *(u32 *)r1;
    r3 = 0xE8;
    r3 <<= 2;
    r1 = r0 + r3;
    *(u16 *)r1 = r2;
    r1 = 0x3A2;
    r0 += r1;
    *(u16 *)r0 = r2;
}
