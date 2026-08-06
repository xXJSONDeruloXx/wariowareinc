#include "global.h"
#include "graphics.h"

void func_08039A44(void) {
    register u8 *base asm("r1") = (u8 *)&gGraphicsBuffer;
    register u32 value asm("r2") = *(u16 *)base;
    register u32 masked asm("r0") = 0x7FFF;

    masked &= value;
    value = 0;
    *(u16 *)base = masked;
    masked = (u32)base;
    masked += 0x4C;
    *(u16 *)masked = value;
    base += 0x4E;
    *(u16 *)base = value;
}
