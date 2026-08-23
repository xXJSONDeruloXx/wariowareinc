#include "global.h"

extern u8 *D_030068E8;

void func_080F0DE0(u32 arg0) {
    u8 *base = D_030068E8;
    u32 offset = arg0;
    offset <<= 5;
    offset += (u32)base;
    *(u32 *)(offset + 0xC) = 0;
    *(u8 *)offset |= 1;
}
