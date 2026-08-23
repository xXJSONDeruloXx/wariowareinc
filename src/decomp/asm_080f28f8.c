#include "global.h"

u32 func_080F28F8(u32 value) {
    u32 result;

    result = value;
    result >>= 3;
    value = result;
    value >>= 1;
    result += value;
    if (result > 0xF)
        result = 0xF;
    return result;
}
