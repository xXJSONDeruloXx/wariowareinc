#include "global.h"

u32 func_080F2C68(u8 *arg0) {
    u8 *base;
    u32 count;
    u32 value;

    base = arg0;
    count = 0;
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
    return count;
}
