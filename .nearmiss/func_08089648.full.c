#include "global.h"

s32 func_08089648(void *arg0, u32 arg1) {
    u8 *p = (u8 *)arg0;
    s32 changed = 0;

    if ((u32)(*(u32 *)((u8 *)gCurrentSceneVariable + 0x3C) -
              *(u32 *)(p + 0x38)) < arg1) {
        changed = 1;
    }
    return changed;
}
