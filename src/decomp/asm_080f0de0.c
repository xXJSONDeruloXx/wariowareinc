#include "global.h"

extern u8 *D_030068E8;

void func_080F0DE0(u32 arg0) {
    register u8 *base asm("r1") = D_030068E8;
    register u32 offset asm("r0") = arg0;
    offset <<= 5;
    offset += (u32)base;
    *(u32 *)(offset + 0xC) = 0;
    *(u8 *)offset |= 1;
}
