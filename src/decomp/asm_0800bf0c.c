#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "graphics.h"

void func_0800BF0C(s32 arg0) {
    gGraphicsBuffer.DISPCNT |= (0x80 << 1) << arg0;
}
#endif
