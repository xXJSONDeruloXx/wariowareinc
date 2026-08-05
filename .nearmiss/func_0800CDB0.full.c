#include "global.h"

void func_0800CDB0(u32 arg0) {
    u8 *base = (u8 *)&gBeatscriptScene;
    base[2] = (u8)((-3 & base[2]) | ((arg0 & 1) << 1));
}
