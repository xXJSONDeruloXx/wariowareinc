#if __INCLUDE_LEVEL__ > 0
#include "global.h"

s32 func_08002468(u8 *ptr) {
    u32 result;
    result = *ptr;
    result <<= 31;
    result >>= 31;
    return result;
}
#endif
