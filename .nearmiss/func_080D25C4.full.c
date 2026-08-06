#include "global.h"

s32 func_080D25C4(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;

    if (p[0x3DC] == 0) {
        return 1;
    }
    return 0;
}
