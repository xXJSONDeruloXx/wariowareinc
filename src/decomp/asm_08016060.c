#include "global.h"
#include "src/memory.h"

s32 func_08016060(void) {
    s32 ready;

    if (save_is_stage_unlocked(0xD) == 0) {
        ready = 0;
        if (func_08008AA4(0xC) > 0xE) {
            ready = 1;
        }
        if (ready != 0) {
            func_080006A4(0xD);
            save_unlock_stage(0xD);
            return 0x80 << 6;
        }
    }
    return 0;
}
