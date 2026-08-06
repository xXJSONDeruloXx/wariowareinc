#include "global.h"
#include "types.h"

u8 func_08030F9C(u32 arg0, u32 arg1) {
    u8 *base = (u8 *)gCurrentSceneVariable;
    u8 *table = *(u8 **)(base + 0x14);

    return table[arg1 * 0xE + arg0];
}
