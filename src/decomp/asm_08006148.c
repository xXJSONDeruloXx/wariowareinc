#include "global.h"

void func_08006148(void *arg0, u32 arg1) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1") = arg1;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r2 = 0x03000BE0;
    *(u32 *)r2 = r0;
    r3 = 0x03000BE4;
    r2 = r1 >> 2;
    *(u32 *)r3 = r2;
    r2 -= 1;
    *(u32 *)r0 = r2;
    r2 = 0x03003FD0;
    r3 = 0;
    *(u32 *)r2 = r3;
    *(u32 *)(r2 + 4) = r0;
    *(u32 *)(r2 + 8) = r1;
    *(u32 *)(r2 + 0x10) = r3;
    *(u32 *)(r2 + 0xC) = r3;
}
