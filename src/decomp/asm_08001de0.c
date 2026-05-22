#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 D_03000010[];

u32 func_08001DE0(s32 arg0) {
    u32 shifted;
    u32 *base;
    if (arg0 < 0) {
        return 0;
    }
    shifted = arg0 << 3;
    base = D_03000010;
    return (u32)((u8 *)base + shifted);
}
#endif
