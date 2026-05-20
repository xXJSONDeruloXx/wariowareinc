#include "global.h"

void func_080BB344(void *arg0, u32 unused, u32 mul, u32 add) {
    u32 *fields = (u32 *)arg0;
    u32 val = fields[0x4 / 4];
    val = ((s32)val * (s32)mul) >> 8;
    val += add;
    fields[0x8 / 4] = val;
}