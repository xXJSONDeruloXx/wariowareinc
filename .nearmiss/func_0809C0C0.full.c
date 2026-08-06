#include "global.h"

s32 func_0809C0C0(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;

    if (*(u16 *)(p + 0x1E) > 0x1FFF) {
        return 0;
    }
    return 3;
}
