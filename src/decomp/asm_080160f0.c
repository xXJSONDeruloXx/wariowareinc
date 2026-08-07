#include "global.h"
#include "src/memory.h"

s32 func_080160F0(void) {
    if (save_is_stage_unlocked(0x10) == 0) {
        if (func_08008AA4(1) > 0x13) {
            save_unlock_stage(0x10);
            return 0x80 << 9;
        }
    }
    return 0;
}
