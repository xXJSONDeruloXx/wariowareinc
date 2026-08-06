#include "global.h"

s32 func_08089648(void *arg0, u32 arg1) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1") = arg1;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3") = 0;

    r2 = *(u32 *)(r0 + 0x38);
    r0 = (u32)&gCurrentSceneVariable;
    r0 = *(u32 *)r0;
    r0 = *(u32 *)(r0 + 0x3C);
    r0 -= r2;
    if (r0 < r1) {
        r3 = 1;
    }
    return r3;
}
