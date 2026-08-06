#include "global.h"

s32 func_080D750C(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;

    if (p[0x43A] == 4) {
        return 1;
    }
    return 0;
}
