#include "global.h"

void func_08003D28(u8 *arg0, s32 arg1) {
    register s32 r1 asm("r1") = arg1;
    register s32 r2 asm("r2") = 1;
    register s32 r3 asm("r3");

    r1 &= r2;
    r1 <<= 2;
    r3 = *arg0;
    r2 = 5;
    asm volatile("" : "+r"(r2));
    r2 = -r2;
    r2 &= r3;
    r2 |= r1;
    *arg0 = r2;
}
