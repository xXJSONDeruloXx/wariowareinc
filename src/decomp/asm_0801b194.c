#include "global.h"

void func_0801B194(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    u32 mask = 3;
    u32 val = p[0x19];
    mask = -mask;
    mask = val & mask;
    p[0x19] = (u8)mask;
}
