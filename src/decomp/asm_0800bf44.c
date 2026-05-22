#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u16 D_03004004[];

void func_0800BF44(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    u16 *base;
    u16 *ptr;
    u16 val;
    base = D_03004004;
    ptr = base + arg0;
    val = (arg1 << 2) | (arg2 << 8) | arg3;
    *ptr = val;
}
#endif
