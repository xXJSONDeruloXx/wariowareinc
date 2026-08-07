#include "global.h"
#include "src/memory.h"

s32 func_08015EB8(void) {
    if (save_is_stage_unlocked(4) == 0) {
        if (func_0800068C(9) != 0) {
            save_unlock_stage(4);
            return 0x10;
        }
    }
    return 0;
}
