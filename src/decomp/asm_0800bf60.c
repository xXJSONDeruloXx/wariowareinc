#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u16 D_03004004[];

void func_0800BF60(u32 arg0, u32 arg1) {
    u16 *base;
    u16 *ptr;
    u16 loaded;
    u32 mask;
    base = D_03004004;
    ptr = base + arg0;
    loaded = *ptr;
    mask = 0xFFFC;
    mask = mask & loaded;
    mask = mask | arg1;
    *ptr = mask;
}
#endif
