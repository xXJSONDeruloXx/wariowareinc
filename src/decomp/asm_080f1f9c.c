#include "global.h"

u32 func_080F1F9C(u32 value) {
    u8 v = (u8)value;
    if (v > 0x3F)
        return 0x7F;
    return (u8)(v << 1);
}
