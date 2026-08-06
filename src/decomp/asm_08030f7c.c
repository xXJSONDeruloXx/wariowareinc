#include "global.h"
#include "types.h"

void func_08030F7C(u32 arg0, u32 arg1, u32 arg2) {
    u8 *base = (u8 *)gCurrentSceneVariable;
    u8 *table = *(u8 **)(base + 0x14);

    table[arg1 * 0xE + arg0] = arg2;
}
