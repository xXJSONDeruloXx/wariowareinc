#include "scenes.h"

void func_08016A7C(s32 arg0) {
    register u8 *data asm("r2") = (u8 *)gCurrentSceneData;
    register u32 mask asm("r1") = 3;
    register u32 arg asm("r0") = arg0;
    u32 value;
    data += 0x4A;
    arg = (arg & 1) << 1;
    value = *data;
    mask = -mask;
    mask &= value;
    mask |= arg;
    *data = mask;
}
