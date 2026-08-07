#include "global.h"
#include "src/memory.h"

s32 func_08016328(void) {
    if (save_is_stage_unlocked(0x15) == 0) {
        if (func_0800068C(0xA) != 0) {
            func_080006A4(0x15);
            save_unlock_stage(0x15);
            return 0x80 << 0xE;
        }
    }
    return 0;
}
