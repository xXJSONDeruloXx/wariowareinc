#include "global.h"
#include "src/memory.h"

s32 func_08015EE0(void) {
    if (save_is_stage_unlocked(5) == 0) {
        if (func_0800068C(1) != 0) {
            save_unlock_stage(5);
            return 0x20;
        }
    }
    return 0;
}
