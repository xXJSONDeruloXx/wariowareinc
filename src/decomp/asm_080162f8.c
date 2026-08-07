#include "global.h"
#include "src/memory.h"

s32 func_080162F8(void) {
    if (save_is_stage_unlocked(0x14) == 0) {
        if (func_0800068C(1) != 0) {
            func_080006A4(0x14);
            save_unlock_stage(0x14);
            return 0x80 << 0xD;
        }
    }
    return 0;
}
