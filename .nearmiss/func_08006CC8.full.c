#include "global.h"
#include "graphics.h"

void func_08006CC8(void) {
    register u8 *base asm("r2") = (u8 *)&gGraphicsBuffer;
    register u32 mask asm("r0") = 0x854;
    register u32 value asm("r1");

    base += mask;
    value = *base;
    mask = 3;
    mask = -mask;
    mask &= value;
    value = 9;
    value = -value;
    mask &= value;
    *base = mask;
}
