#include "global.h"

s32 func_0806EC7C(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;

    if (p[0x3B] == p[0x3A]) {
        return 1;
    }
    return 0;
}
