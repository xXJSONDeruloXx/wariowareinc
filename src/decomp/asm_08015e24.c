#include "global.h"
#include "src/memory.h"

s32 func_08015E24(void) {
    if (save_is_stage_unlocked(0) != 0) {
        return 0;
    }
    save_unlock_stage(0);
    return 1;
}
