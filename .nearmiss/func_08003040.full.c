#include "global.h"

extern void func_08002FE8(void *, void *, u32);

void func_08003040(void *arg0, void *arg1, u32 arg2) {
    register u32 *r3 asm("r3") = arg0;
    register u32 *r1 asm("r1") = arg1;
    register u32 r2 asm("r2") = arg2;
    register u32 r0 asm("r0");

    goto check;
loop:
    r3 = (u32 *)((u8 *)r3 + 8);
check:
    r0 = *(u32 *)((u8 *)r3 + 0);
    if (r0 != 0) goto loop;
    r0 = (u32)r3;
    func_08002FE8((void *)r0, (void *)r1, r2);
}
