#include "global.h"

void func_080D2450(s32 arg0, s32 arg1) {
    u32 offset = (u32)arg0 << 24;
    u8 *base = (u8 *)gCurrentSceneVariable;

    offset >>= 22;
    base += 0x3AC;
    base += offset;
    *(s32 *)base = (arg1 << 16) >> 8;
}
