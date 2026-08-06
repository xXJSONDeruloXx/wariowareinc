#include "global.h"

s32 func_080A7A74(void) {
    if (((u8 *)gCurrentSceneVariable)[0xB] != 0) {
        return 0;
    }
    return 1;
}
