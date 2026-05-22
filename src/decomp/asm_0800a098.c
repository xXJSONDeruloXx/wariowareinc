#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;

u32 func_0800A098(void) {
    register void **base asm("r4");
    register u8 *ptr asm("r2");
    register u32 offset asm("r3");
    register u8 *reload asm("r1");
    u8 value;

    base = &gCurrentSceneData;
    ptr = *base;
    offset = 0x175;
    ptr += offset;
    value = *ptr;
    value++;
    *ptr = value;

    reload = *base;
    ptr = reload + offset;
    value = *ptr;
    if (value > 4) {
        value = 4;
        *ptr = value;
    }
}
#endif
