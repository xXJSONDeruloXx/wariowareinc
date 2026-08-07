#include "global.h"
#include "src/memory.h"

s32 func_080162B0(void) {
    s32 temp;
    u32 count;

    if (save_is_stage_unlocked(0x1A) == 0) {
        temp = func_0800068C(2);
        count = (u32)((0 - temp) | temp) >> 0x1F;
        if (func_0800068C(3) != 0) {
            count += 1;
        }
        if (func_0800068C(5) != 0) {
            count += 1;
        }
        if (count > 1) {
            save_unlock_stage(0x1A);
            return 0x80 << 0x13;
        }
    }
    return 0;
}
