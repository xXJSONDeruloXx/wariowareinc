#include "global.h"
#include "types.h"

void func_08076378(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    *(u32 *)(p + 0x20) = 0;
    *(u8 *)(p + 0x1C) = 1;
}
