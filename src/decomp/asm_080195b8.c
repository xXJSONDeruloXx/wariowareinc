#include "global.h"
#include "graphics.h"

void func_080195B8(u32 arg0) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r1 = (u32)&gCurrentSceneVariable;
    r1 = *(u32 *)r1;
    r2 = r1;
    r2 += 0x66;
    r3 = 1;
    *(u16 *)r2 = r3;
    r2 -= 4;
    *(u16 *)r2 = r0;
    r1 += 0x64;
    *(u16 *)r1 = r0;
    r0 = (u32)&gGraphicsBuffer;
    r2 = r0;
    r2 += 0x4C;
    r1 = 0xBF;
    *(u16 *)r2 = r1;
    r0 += 0x50;
    *(u16 *)r0 = r3;
}
