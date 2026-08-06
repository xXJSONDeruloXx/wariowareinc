#include "global.h"

void func_080DD8A4(void) {
    register u32 r1 asm("r1") = (u32)&gCurrentSceneVariable;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r2 = *(u32 *)r1;
    r1 = 0x10;
    r3 = *(s16 *)(r2 + r1);
    r1 = *(u32 *)(r2 + 8);
    r1 += r3;
    *(u32 *)(r2 + 8) = r1;
    r1 = 0x12;
    r3 = *(s16 *)(r2 + r1);
    r1 = *(u32 *)(r2 + 0xC);
    r1 += r3;
    *(u32 *)(r2 + 0xC) = r1;
}
