#include "global.h"

u8 *func_08007AD4(u8 *arg0, u8 *arg1, u32 arg2) {
    register u8 *r0 asm("r0") = arg0;
    register u8 *r1 asm("r1") = arg1;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4") = arg2;
    register u8 *r5 asm("r5") = arg0;

    r2 = 0;
    goto check;
loop:
    *r0 = r3;
    r1 += 1;
    r0 += 1;
    r2 += 1;
check:
    r3 = *r1;
    if (r3 != 0 && r2 < r4) goto loop;
    r0 = r5;
    return r0;
}
