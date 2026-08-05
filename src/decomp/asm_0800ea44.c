#include "global.h"
#include "scenes.h"

void func_0800EA44(s32 arg0) {
    u8 *ptr = (u8 *)gCurrentSceneData;
    ptr += 0x14D;
    ptr += arg0;
    *ptr = 1;
}
