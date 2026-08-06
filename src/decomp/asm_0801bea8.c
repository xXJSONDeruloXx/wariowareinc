#include "global.h"

void func_0801BEA8(void) {
    u8 *data = (u8 *)gCurrentSceneVariable;
    register u32 value asm("r1");
    register u32 mask asm("r0") = 0x3D;
    value = data[0x18];
    mask = -mask;
    mask &= value;
    mask |= 0x14;
    data[0x18] = mask;
}
