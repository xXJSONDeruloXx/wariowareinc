#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 save_is_stage_unlocked(u32);

u32 func_0801274C(u32 arg0) {
    u32 result;
    result = save_is_stage_unlocked(arg0);
    if (result != 0) {
        return 1;
    }
    if (arg0 <= 0xA) {
        return 1;
    }
    return 0;
}
#endif
