#include "global.h"

void func_080F2598(void *arg0, s32 arg1, u8 arg2) {
    register u32 value asm("r2") = arg2;
    register u32 base asm("r0");
    register u32 offset asm("r1") = arg1;
    register u32 field asm("r3");

    value <<= 24;
    value >>= 24;
    asm("" ::: "memory");
    base = *(u32 *)((u8 *)arg0 + 0x18);
    offset <<= 5;
    offset += base;
    value &= 1;
    value <<= 6;
    field = *(u8 *)(offset + 3);
    asm("" ::: "memory");
    base = 0x41;
    base = -base;
    base &= field;
    base |= value;
    *(u8 *)(offset + 3) = base;
}
