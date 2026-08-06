#include "global.h"
#include "graphics.h"

void func_08039A44(void) {
    u8 *base = (u8 *)&gGraphicsBuffer;
    u16 value = *(u16 *)base;

    value &= 0x7FFF;
    *(u16 *)base = value;
    base += 0x4C;
    *(u16 *)base = 0;
    base += 2;
    *(u16 *)base = 0;
}
