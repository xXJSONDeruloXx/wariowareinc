#include "global.h"
#include "src/memory.h"

s32 func_08016118(void) {
    if (save_is_stage_unlocked(0xF) == 0) {
        if (func_08008AA4(0xA) > 0x18) {
            save_unlock_stage(0xF);
            return 0x80 << 8;
        }
    }
    return 0;
}
