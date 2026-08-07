#include "global.h"
#include "src/memory.h"

s32 func_08016358(void) {
    if (save_is_stage_unlocked(0x16) == 0) {
        if (func_0800068C(9) != 0) {
            func_080006A4(0x16);
            save_unlock_stage(0x16);
            return 0x80 << 0xF;
        }
    }
    return 0;
}
