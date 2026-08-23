#include "global.h"

u32 func_080F282C(u16 arg0) {
    u32 value;
    u16 *counter;
    u32 field;
    u32 factor;
    u16 reload;

    value = arg0;
    value <<= 16;
    value >>= 16;
    counter = (u16 *)0x03000E78;
    field = *counter;
    factor = 0x6D;
    factor *= field;
    field = 0x3FD;
    factor += field;
    *counter = factor;
    reload = *counter;
    value *= reload;
    value >>= 16;
    return value;
}
