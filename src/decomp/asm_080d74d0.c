#include "global.h"

s32 func_080D74D0(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;

    if (p[0x43A] == 0) {
        return 1;
    }
    return 0;
}
