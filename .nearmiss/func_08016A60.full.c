#include "scenes.h"

void func_08016A60(s32 arg0) {
    register u8 *data asm("r2") = (u8 *)gCurrentSceneData;
    register u32 mask asm("r1") = 2;
    u32 value;
    data += 0x4A;
    arg0 &= 1;
    value = *data;
    mask = -mask;
    mask &= value;
    mask |= arg0;
    *data = mask;
}
