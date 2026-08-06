#include "global.h"

s32 func_080A7A74(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;

    if (p[0xB] == 0) {
        return 1;
    }
    return 0;
}
