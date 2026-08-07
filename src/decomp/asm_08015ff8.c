#include "global.h"
#include "src/memory.h"

s32 func_08015FF8(void) {
    if (save_is_stage_unlocked(0xB) == 0) {
        if (func_0800068C(8) != 0) {
            func_080006A4(0xB);
            save_unlock_stage(0xB);
            return 0x80 << 4;
        }
    }
    return 0;
}
