#if __INCLUDE_LEVEL__ > 0
#include "global.h"

s32 func_0800210C(s32 arg0) {
    s32 result;
    s32 addr;
    result = arg0;
    if (arg0 < 0) {
        addr = arg0 & 0x7FFFFFFF;
        result = *(s32 *)addr;
    }
    return result;
}
#endif
