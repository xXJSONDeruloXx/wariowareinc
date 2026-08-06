#include "global.h"

void func_0801B3E4(void) {
    u8 *data = (u8 *)gCurrentSceneVariable;
    register u32 value asm("r2");
    register u32 mask asm("r0") = 2;
    value = data[0xF4];
    mask = -mask;
    mask &= value;
    data[0xF4] = mask;
}
