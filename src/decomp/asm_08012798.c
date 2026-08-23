#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 func_0801274C(u32);
extern u8 D_083AA0C4[];

s32 func_08012798(u32 arg0) {
    u32 index;
    u32 table;
    s32 value;

    table = (u32)D_083AA0C4;
    index = arg0;
    index <<= 4;
    goto check;
body:
    if (func_0801274C(value) != 0) {
        return value;
    }
    index = value;
    index <<= 4;
check:
    index += table;
    value = *(s8 *)(index + 5);
    if (value >= 0) {
        goto body;
    }
    return -1;
}
#endif
