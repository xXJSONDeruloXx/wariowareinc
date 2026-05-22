#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 D_0300400C[];

void func_0800BF34(s32 arg0, s32 arg1, s32 arg2) {
    u32 *base;
    u16 *ptr;
    base = D_0300400C;
    ptr = (u16 *)((u8 *)base + (arg0 << 2));
    ptr[0] = arg1;
    ptr[1] = arg2;
}
#endif
