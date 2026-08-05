#include "global.h"

s32 func_0805627C(s32 arg0) {
    s32 value = (s16)arg0;

    if (value < 0) {
        value += 3;
    }
    return 3 - (value >> 2);
}
