#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern s32 func_08001AC0(u32);
extern u32 D_03000140[];

s32 func_08001B04(u32 arg0) {
    u32 saved;
    s32 result;
    u32 *base;

    saved = arg0;
    result = func_08001AC0(saved);
    if (result >= 0) {
        base = D_03000140;
        base[result] = saved;
    }
    return result;
}
#endif
