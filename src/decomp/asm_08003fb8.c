#include "global.h"

extern u8 D_03000528;

void func_08003FB8(void) {
    u8 *base;
    u8 value;
    u32 mask;
    u32 other;

    base = &D_03000528;
    value = *base;
    mask = 2;
    mask = -mask;
    mask &= value;
    other = 9;
    other = -other;
    mask &= other;
    *base = mask;
}
