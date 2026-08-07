#include "global.h"
#include "src/memory.h"

s32 func_080161D8(void) {
    s32 temp;
    u32 count;

    if (save_is_stage_unlocked(0x17) == 0) {
        temp = func_0800068C(4);
        count = (u32)((0 - temp) | temp) >> 0x1F;
        if (func_0800068C(6) != 0) {
            count += 1;
        }
        if (func_0800068C(7) != 0) {
            count += 1;
        }
        if (count != 0) {
            save_unlock_stage(0x17);
            return 0x80 << 0x10;
        }
    }
    return 0;
}
