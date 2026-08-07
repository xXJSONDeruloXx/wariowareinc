#include "global.h"
#include "src/memory.h"

s32 func_08015E68(void) {
    if (save_is_stage_unlocked(2) == 0) {
        if (func_0800068C(1) != 0) {
            save_unlock_stage(2);
            return 4;
        }
    }
    return 0;
}
