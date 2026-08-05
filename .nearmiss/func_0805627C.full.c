#include "global.h"

s32 func_0805627C(s16 arg0) {
    s16 value = arg0;

    if (value < 0) {
        value += 3;
    }
    return 3 - (value >> 2);
}
