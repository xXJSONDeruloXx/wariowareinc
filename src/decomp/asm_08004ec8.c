#include "global.h"

extern void func_08004EE8(void *);

void func_08004EC8(void *arg0, u32 arg1, s8 arg2, s16 arg3,
                   u32 arg4, u32 arg5, u32 arg6) {
    u8 *base = (u8 *)arg0;

    *(u32 *)(base + 0x0) = arg1;
    *(s8 *)(base + 0x6) = arg2;
    *(s16 *)(base + 0x4) = arg3;
    *(s8 *)(base + 0x7) = (s8)arg4;
    *(u32 *)(base + 0x8) = arg5;
    *(u32 *)(base + 0xC) = arg6;
    func_08004EE8(arg0);
}
