#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "graphics.h"

void func_0800BEF4(u32 arg0) {
    u16 *base;
    u16 loaded;
    u32 mask;
    base = (u16 *)&gGraphicsBuffer;
    loaded = base[0];
    mask = 0xFFF8;
    mask = mask & loaded;
    mask = mask | arg0;
    base[0] = mask;
}
#endif
