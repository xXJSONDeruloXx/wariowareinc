#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

void func_0800A3A4(s32 arg0) {
    u8 *ptr;
    u8 val;
    u32 shifted;
    ptr = (u8 *)gCurrentSceneData;
    shifted = arg0 << 7;
    val = ptr[6] & 0x7F;
    val |= shifted;
    ptr[6] = val;
}
#endif
