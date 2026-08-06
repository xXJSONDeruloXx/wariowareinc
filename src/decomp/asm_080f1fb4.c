#include "global.h"

u8 func_080F1FB4(u8 arg0) {
    register u32 value asm("r0") = arg0;
    register u32 temp asm("r1");

    value <<= 24;
    temp = value >> 24;
    if (temp <= 0x3F)
        return 0x7F;
    value = 0x7F;
    value -= temp;
    value <<= 25;
    value >>= 24;
    return value;
}
