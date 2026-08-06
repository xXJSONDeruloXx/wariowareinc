#include "global.h"

void func_0804F464(u32 *arg0, u32 arg1, u32 arg2, u32 arg3) {
    u8 *ptr = (u8 *)arg0;
    s32 mask = 0;

    *(u32 *)(ptr + 4) = arg1;
    *(u32 *)(ptr + 8) = arg2;
    *(u32 *)(ptr + 0x0C) = arg3;
    *(u32 *)(ptr + 0x14) = 0;
    *(u32 *)(ptr + 0x10) = 0;
    *(u32 *)(ptr + 0x18) = 0;
    *(u32 *)(ptr + 0x1C) = 0;
    ptr += 0x20;
    mask -= 0x10;
    *ptr &= (u8)mask;
}
