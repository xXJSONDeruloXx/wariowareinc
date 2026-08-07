#include "global.h"
#include "src/memory.h"

s32 func_08016388(void) {
    if (save_is_stage_unlocked(0x1B) == 0) {
        if (func_0800068C(8) != 0) {
            func_080006A4(0x1B);
            save_unlock_stage(0x1B);
            return 0x80 << 0x14;
        }
    }
    return 0;
}
