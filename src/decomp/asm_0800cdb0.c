#include "global.h"

void func_0800CDB0(s32 arg0) {
    u8 *base = (u8 *)&gBeatscriptScene;
    u32 value = arg0;
    u32 current;
    u32 mask;

    value &= 1;
    value <<= 1;
    current = base[2];
    mask = 3;
    asm("" : "+r"(mask));
    mask = -mask;
    mask &= current;
    base[2] = (u8)(mask | value);
}
