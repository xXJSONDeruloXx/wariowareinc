#include "global.h"
#include "src/memory.h"

s32 func_08016098(void) {
    if (save_is_stage_unlocked(0xE) == 0) {
        if (func_0800068C(8) != 0) {
            func_080006A4(0xE);
            save_unlock_stage(0xE);
            return 0x80 << 7;
        }
    }
    return 0;
}
