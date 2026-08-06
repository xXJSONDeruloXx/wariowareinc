#include "global.h"

extern u8 *volatile D_030068E8;

void func_080F0E14(u32 arg0, u8 arg1, u8 arg2) {
    register u32 offset asm("r0") = arg0;
    offset <<= 5;
    D_030068E8[offset + 2] = arg1;
    D_030068E8[offset + 3] = arg2;
}
