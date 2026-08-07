#include "global.h"
#include "src/memory.h"

s32 func_08016268(void) {
    s32 temp;
    u32 count;

    if (save_is_stage_unlocked(0x19) == 0) {
        temp = func_0800068C(2);
        count = (u32)((0 - temp) | temp) >> 0x1F;
        if (func_0800068C(3) != 0) {
            count += 1;
        }
        if (func_0800068C(5) != 0) {
            count += 1;
        }
        if (count != 0) {
            save_unlock_stage(0x19);
            return 0x80 << 0x12;
        }
    }
    return 0;
}
