#include "global.h"

u32 func_080F2FFC(u16 arg0, u16 arg1, u16 arg2) {
    u32 value = (u32)arg1 * (u32)arg0;

    value *= (u32)arg2;
    return value / 3600U;
}
