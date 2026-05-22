#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

void func_0800A280(s32 arg0) {
    u8 *base;
    u8 *ptr;
    u8 val;
    base = (u8 *)&gBeatscriptScene;
    ptr = base + arg0 * 0x9C;
    ptr = ptr + 0x28;
    val = *ptr;
    val |= 0x80;
    *ptr = val;
}
#endif
