#include "global.h"
#include "src/memory.h"

s32 func_0801618C(void) {
    u32 count;
    u32 i;
    u8 *flags;

    if (save_is_stage_unlocked(0x13) == 0) {
        count = 0;
        flags = gSaveBuffer->microgameFlags;
        i = 0;
        do {
            if ((flags[i] & 2) != 0) {
                count++;
            }
            i++;
        } while (i <= 0xE1);
        if (count > 0xD4) {
            save_unlock_stage(0x13);
            return 0x80 << 0xC;
        }
    }
    return 0;
}
