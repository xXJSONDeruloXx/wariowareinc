#include "global.h"
#include "src/memory.h"

s32 func_08015E90(void) {
    if (save_is_stage_unlocked(3) == 0) {
        if (func_0800068C(1) != 0) {
            save_unlock_stage(3);
            return 8;
        }
    }
    return 0;
}
