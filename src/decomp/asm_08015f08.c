#include "global.h"
#include "src/memory.h"

s32 func_08015F08(void) {
    if (save_is_stage_unlocked(6) == 0) {
        if (func_0800068C(9) != 0) {
            save_unlock_stage(6);
            return 0x40;
        }
    }
    return 0;
}
