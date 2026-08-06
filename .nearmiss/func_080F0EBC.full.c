#include "global.h"

extern u8 *D_030068E8;

void func_080F0EBC(u32 arg0, u32 arg1) {
    register u32 base asm("r2") = (u32)D_030068E8;
    register u32 offset asm("r0") = arg0;
    register u32 value asm("r1") = arg1;
    register u32 current asm("r3");
    offset <<= 5;
    offset += base;
    base = 1;
    value &= base;
    value <<= 3;
    current = *(u8 *)offset;
    base = 9;
    base = -base;
    base &= current;
    base |= value;
    *(u8 *)offset = base;
}
