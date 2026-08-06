#include "global.h"
#include "scenes.h"

s32 func_08016F60(void) {
    u8 value = *(u8 *)((u8 *)gCurrentSceneData + 4);
    if (value != 0)
        return 1;
    return 0;
}
