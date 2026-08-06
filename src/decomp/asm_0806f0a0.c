#include "global.h"

void func_0806F0A0(void) {
    register u32 r0 asm("r0") = (u32)&gCurrentSceneVariable;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");

    r2 = *(u32 *)r0;
    r0 = 0xC;
    r1 = *(s16 *)(r2 + r0);
    r0 = *(u32 *)(r2 + 4);
    r0 += r1;
    *(u32 *)(r2 + 4) = r0;
    r1 = 0xD0;
    r1 <<= 8;
    if ((s32)r0 > (s32)r1) {
        *(u32 *)(r2 + 4) = r1;
    }
}
