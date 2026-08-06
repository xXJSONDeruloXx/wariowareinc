#include "global.h"

extern void func_08002FC0(void *, void *);

void func_08003028(void *arg0, void *arg1) {
    register u32 *r2 asm("r2") = arg0;
    register u32 *r1 asm("r1") = arg1;
    register u32 r0 asm("r0");

    goto check;
loop:
    r2 = (u32 *)((u8 *)r2 + 8);
check:
    r0 = *(u32 *)((u8 *)r2 + 0);
    if (r0 != 0) goto loop;
    r0 = (u32)r2;
    func_08002FC0((void *)r0, (void *)r1);
}
