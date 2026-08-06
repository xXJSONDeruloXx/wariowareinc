#include "global.h"

s32 func_08008058(s32 value, s32 minimum, s32 maximum) {
    if (value < minimum)
        value = minimum;
    else if (value > maximum)
        value = maximum;
    return value;
}
