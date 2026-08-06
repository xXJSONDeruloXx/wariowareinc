#include "global.h"

s32 func_08088B80(void *arg0) {
    u8 *p = (u8 *)arg0;
    s32 changed = 0;

    if (*(s32 *)(p + 8) > 0x7800) {
        *(s32 *)(p + 8) = 0x7800;
        changed = 1;
    }
    return changed;
}
