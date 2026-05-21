#include "global.h"

void func_0800CCB4(void) {
    u8 *p = (u8 *)&gBeatscriptScene;
    u32 mask = 2;
    u32 val = p[2];
    mask = -mask;
    mask = val & mask;
    p[2] = (u8)mask;
}
