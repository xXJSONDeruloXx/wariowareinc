#include "global.h"

void func_08002FC0(void *arg0, void *arg1) {
    register u32 *r2 asm("r2") = arg0;
    register u32 *r1 asm("r1") = arg1;
    register u32 r0 asm("r0");

    goto check;
loop:
    r0 = *(u32 *)((u8 *)r1 + 0);
    *(u32 *)((u8 *)r2 + 0) = r0;
    r0 = *(u8 *)((u8 *)r1 + 4);
    *(u8 *)((u8 *)r2 + 4) = r0;
    r0 = *(u8 *)((u8 *)r1 + 5);
    *(u8 *)((u8 *)r2 + 5) = r0;
    r2 = (u32 *)((u8 *)r2 + 8);
    r1 = (u32 *)((u8 *)r1 + 8);
check:
    r0 = *(u32 *)((u8 *)r1 + 0);
    if (r0 != 0) goto loop;
    r0 = 0;
    *(u32 *)((u8 *)r2 + 0) = r0;
    *(u8 *)((u8 *)r2 + 5) = r0;
    *(u8 *)((u8 *)r2 + 4) = r0;
}
