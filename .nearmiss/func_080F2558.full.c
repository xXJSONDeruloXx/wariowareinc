#include "global.h"

void func_080F2558(void *arg0, s32 arg1, u8 arg2) {
    register u32 value asm("r2") = arg2;
    register u32 base asm("r0") = *(u32 *)((u8 *)arg0 + 0x18);
    register u32 offset asm("r1") = arg1;
    register u32 field asm("r3");

    value <<= 24;
    value >>= 24;
    offset <<= 5;
    offset += base;
    base = 0x7F;
    value &= base;
    value <<= 14;
    field = *(u32 *)(offset + 4);
    base = 0xFFE03FFF;
    base &= field;
    base |= value;
    *(u32 *)(offset + 4) = base;
}
