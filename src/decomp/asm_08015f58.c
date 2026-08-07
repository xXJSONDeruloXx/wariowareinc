#include "global.h"
#include "src/memory.h"

s32 func_08015F58(void) {
    if (save_is_stage_unlocked(8) == 0) {
        if (func_0800068C(0xA) != 0) {
            save_unlock_stage(8);
            return 0x80 << 1;
        }
    }
    return 0;
}
