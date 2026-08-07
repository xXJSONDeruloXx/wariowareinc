#include "global.h"
#include "src/memory.h"

s32 func_08015F30(void) {
    if (save_is_stage_unlocked(7) == 0) {
        if (func_0800068C(9) != 0) {
            save_unlock_stage(7);
            return 0x80;
        }
    }
    return 0;
}
