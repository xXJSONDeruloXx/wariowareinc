#include "global.h"
#include "src/memory.h"

s32 func_08015F80(void) {
    if (save_is_stage_unlocked(9) == 0) {
        if (func_0800068C(2) != 0) {
            if (func_0800068C(3) != 0) {
                if (func_0800068C(5) != 0) {
                    save_unlock_stage(9);
                    return 128 << 2;
                }
            }
        }
    }
    return 0;
}
