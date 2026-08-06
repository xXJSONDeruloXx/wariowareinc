#include "global.h"
#include "types.h"

void func_0801C2D4(u32 arg0) {
    register u32 r2 asm("r2") = (u32)&gCurrentSceneVariable;
    register u32 r1 asm("r1");
    register u32 r3 asm("r3");
    register u32 r0 asm("r0") = arg0;

    r1 = *(u32 *)r2;
    r3 = 0x96;
    r3 <<= 1;
    r1 += r3;
    *(u8 *)r1 = r0;
    r1 = *(u32 *)r2;
    r2 = 0x12D;
    r1 += r2;
    *(u8 *)r1 = r0;
}
