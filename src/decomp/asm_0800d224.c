#include "global.h"
#include "types.h"
#include "scenes.h"

void func_0800D224(u32 index, u32 value) {
    u8 *base = (u8 *)&gBeatscriptScene;
    u32 offset = index;
    u32 field = 0x1C5C;
    offset <<= 2;
    base += field;
    offset += (u32)base;
    *(u32 *)offset = value;
}
