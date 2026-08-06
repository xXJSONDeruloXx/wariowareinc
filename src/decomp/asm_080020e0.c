#include "global.h"

extern void func_080F2F78(u32 *, s32, s8);

void func_080020E0(u32 arg0, u8 arg1) {
    u32 value = arg1 << 24;
    u32 normalized = value >> 24;

    if (arg0 != 0) {
        normalized <<= 24;
        normalized = (u32)((s32)normalized >> 24);
        func_080F2F78((u32 *)arg0, (u32)0xFFFF, (s8)normalized);
    }
}
