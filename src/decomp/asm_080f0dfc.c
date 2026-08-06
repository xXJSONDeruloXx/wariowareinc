#include "global.h"

extern u8 *D_030068E8;

void func_080F0DFC(u32 arg0) {
    register u32 base asm("r1") = (u32)D_030068E8;
    register u32 offset asm("r0") = arg0;
    register u32 value asm("r2");
    offset <<= 5;
    offset += base;
    value = *(u8 *)offset;
    base = 2;
    base = -base;
    base &= value;
    *(u8 *)offset = base;
}
