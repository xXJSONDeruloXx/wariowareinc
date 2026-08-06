#include "global.h"

extern void func_08003014();

void func_08003058(void *arg0, u32 arg1, u32 arg2, u32 arg3) {
    register u32 *r4 asm("r4") = arg0;
    register u32 r1 asm("r1") = arg1;
    register u32 r2 asm("r2") = arg2;
    register u32 r3 asm("r3") = arg3;
    register u32 r0 asm("r0");

    goto check;
loop:
    r4 = (u32 *)((u8 *)r4 + 8);
check:
    r0 = *(u32 *)((u8 *)r4 + 0);
    if (r0 != 0) goto loop;
    r0 = (u32)r4;
    func_08003014((void *)r0, r1, r2, r3);
}
