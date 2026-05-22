#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;

void func_0800BCAC(s32 arg0) {
    register u8 *ptr asm("r3") = gCurrentSceneData;
    register u32 shifted asm("r0");
    register u32 mask asm("r1");
    register u8 loaded asm("r2");
    shifted = arg0 & 1;
    shifted <<= 2;
    loaded = ptr[7];
    mask = 5;
    __asm__("" : "+r"(mask));  // Prevent constant folding
    mask = -mask;
    mask &= loaded;
    mask |= shifted;
    ptr[7] = (u8)mask;
}
#endif
