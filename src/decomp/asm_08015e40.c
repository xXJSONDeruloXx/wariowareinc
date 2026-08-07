#include "global.h"
#include "src/memory.h"

s32 func_08015E40(void) {
    if (save_is_stage_unlocked(1) == 0) {
        if (func_0800068C(0) != 0) {
            save_unlock_stage(1);
            return 2;
        }
    }
    return 0;
}
