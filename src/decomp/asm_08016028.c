#include "global.h"
#include "src/memory.h"

s32 func_08016028(void) {
    s32 ready;

    if (save_is_stage_unlocked(0xC) == 0) {
        ready = 0;
        if (func_08008AA4(0xB) > 0xE) {
            ready = 1;
        }
        if (ready != 0) {
            func_080006A4(0xC);
            save_unlock_stage(0xC);
            return 0x80 << 5;
        }
    }
    return 0;
}
