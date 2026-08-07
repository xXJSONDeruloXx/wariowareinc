#include "global.h"
#include "src/memory.h"

s32 func_080160C8(void) {
    if (save_is_stage_unlocked(0x11) == 0) {
        if (func_08008AA4(9) > 0x18) {
            save_unlock_stage(0x11);
            return 0x80 << 0xA;
        }
    }
    return 0;
}
