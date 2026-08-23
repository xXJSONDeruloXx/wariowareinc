#include "global.h"
#include "types.h"

void func_0800CAA4(u32 value) {
    u8 *base = (u8 *)&gBeatscriptScene;
    u32 offset = 0x1C32;
    *(u16 *)(base + offset) = value;
}
