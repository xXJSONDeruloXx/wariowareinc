#include "global.h"
#include "src/memory.h"

s32 func_08015FBC(void) {
    if (save_is_stage_unlocked(0xA) == 0) {
        if (func_0800068C(4) != 0) {
            if (func_0800068C(6) != 0) {
                if (func_0800068C(7) != 0) {
                    save_unlock_stage(0xA);
                    return 0x80 << 3;
                }
            }
        }
    }
    return 0;
}
