#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "graphics.h"

void func_0800BFC8(void) {
    u16 val;
    val = gGraphicsBuffer.DISPCNT;
    val |= 0x80 << 5;
    gGraphicsBuffer.DISPCNT = val;
}
#endif
