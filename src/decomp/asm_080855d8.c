#include "global.h"

s32 func_080855D8(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;

    if (p[0xC5A] != 0) {
        return 1;
    }
    return 0;
}
