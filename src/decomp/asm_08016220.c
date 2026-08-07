#include "global.h"
#include "src/memory.h"

s32 func_08016220(void) {
    s32 temp;
    u32 count;

    if (save_is_stage_unlocked(0x18) == 0) {
        temp = func_0800068C(4);
        count = (u32)((0 - temp) | temp) >> 0x1F;
        if (func_0800068C(6) != 0) {
            count += 1;
        }
        if (func_0800068C(7) != 0) {
            count += 1;
        }
        if (count > 1) {
            save_unlock_stage(0x18);
            return 0x80 << 0x11;
        }
    }
    return 0;
}
