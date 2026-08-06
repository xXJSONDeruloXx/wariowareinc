#include "global.h"

u8 func_080F2C68(u8 *arg0) {
    register u8 *base asm("r2") = arg0;
    register u32 count asm("r1") = 0;
    register u32 value asm("r0");

    value = *base;
    if (value != 0) {
        do {
            value = count + 1;
            value <<= 24;
            value >>= 24;
            count = value;
            value = *(u8 *)(base + count);
        } while (value != 0);
    }
    value = count;
    return value;
}
