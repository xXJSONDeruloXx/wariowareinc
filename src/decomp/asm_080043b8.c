#include "global.h"

extern void func_080043D4(void);

void func_080043B8(void *arg0, u32 arg1, s16 arg2, s8 arg3,
                   u32 unused4, u32 unused5, u32 arg6, u32 arg7) {
    u8 *base = (u8 *)arg0;

    *(u32 *)(base + 4) = arg1;
    *(s16 *)(base + 8) = arg2;
    *(s8 *)(base + 0xA) = arg3;
    *(u32 *)(base + 0xC) = arg6;
    *(u32 *)(base + 0x10) = arg7;
    func_080043D4();
}
