#include "global.h"

s32 func_080D6FF4(void) {
    u8 *p = (u8 *)gCurrentSceneVariable + 8;

    if (p[0x1E] == 3) {
        return 1;
    }
    return 0;
}
