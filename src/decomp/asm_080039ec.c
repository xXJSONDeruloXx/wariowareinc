#include "global.h"

s32 func_080039EC(s32 value) {
    s32 result = (s16)value;
    if (result < 0)
        result = -result;
    return (s16)result;
}
