#include "global.h"
#include "types.h"

void func_080D286C(void) {
    register u32 r2 asm("r2") = (u32)&gCurrentSceneVariable;
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r3 asm("r3");

    r0 = *(u32 *)r2;
    r1 = 0xE6;
    r1 <<= 2;
    r0 += r1;
    r3 = 0;
    r1 = 2;
    *(u8 *)r0 = r1;
    r0 = *(u32 *)r2;
    r1 = 0x39A;
    r0 += r1;
    *(u16 *)r0 = r3;
}
