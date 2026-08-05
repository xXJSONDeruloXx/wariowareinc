#include "global.h"

u32 func_080F2C50(const u8 *arg0) {
    u32 result = (u32)arg0[0] << 24;
    result |= (u32)arg0[1] << 16;
    result |= (u32)arg0[2] << 8;
    result |= arg0[3];
    return result;
}
