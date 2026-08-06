#include "global.h"

void func_080F253C(void *arg0, s32 arg1, u8 arg2) {
    register u32 value asm("r2") = arg2;
    register u32 base asm("r0") = *(u32 *)((u8 *)arg0 + 0x18);
    register u32 offset asm("r1") = arg1;
    register u32 field asm("r3");

    value <<= 24;
    value >>= 24;
    offset <<= 5;
    offset += base;
    base = 1;
    value &= base;
    field = *(u8 *)offset;
    base = 2;
    base = -base;
    base &= field;
    base |= value;
    *(u8 *)offset = base;
}
