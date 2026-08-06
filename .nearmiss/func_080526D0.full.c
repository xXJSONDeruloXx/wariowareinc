#include "global.h"

void func_080526D0(void) {
    register u32 r0 asm("r0") = (u32)&gCurrentSceneVariable;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");

    r1 = *(u32 *)r0;
    r2 = *(s32 *)(r1 + 0x6C);
    r0 = (s32)r2 >> 8;
    if ((s32)r0 <= 0xB3) {
        r0 = *(u32 *)(r1 + 0x70);
        r0 += r2;
        *(u32 *)(r1 + 0x6C) = r0;
    }
}
