#include "global.h"

void func_08002FE8(void *arg0, void *arg1, u32 arg2) {
    register u32 *r3 asm("r3") = arg0;
    register u32 *r1 asm("r1") = arg1;
    register u32 r2 asm("r2") = arg2;
    register u32 r0 asm("r0");

    goto check;
loop:
    *(u32 *)((u8 *)r3 + 0) = r0;
    r0 = *(u8 *)((u8 *)r1 + 4);
    *(u8 *)((u8 *)r3 + 4) = r0;
    r0 = *(u8 *)((u8 *)r1 + 5);
    *(u8 *)((u8 *)r3 + 5) = r0;
    r3 = (u32 *)((u8 *)r3 + 8);
    r1 = (u32 *)((u8 *)r1 + 8);
    r2 -= 1;
check:
    r0 = *(u32 *)((u8 *)r1 + 0);
    if (r0 != 0 && r2 != 0) goto loop;
    r0 = 0;
    *(u32 *)((u8 *)r3 + 0) = r0;
    *(u8 *)((u8 *)r3 + 5) = r0;
    *(u8 *)((u8 *)r3 + 4) = r0;
}
