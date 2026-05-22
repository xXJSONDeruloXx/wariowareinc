#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

void func_0800A200(u32 arg0) {
    u8 *ptr;
    u8 val;
    u32 shifted;
    ptr = (u8 *)gCurrentSceneData;
    shifted = arg0 << 7;
    val = ptr[5] & 0x7F;
    val |= shifted;
    ptr[5] = val;
}
#endif
