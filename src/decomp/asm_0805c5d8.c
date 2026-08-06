#include "global.h"

u8 *func_0805C5D8(u8 *arg0, u8 *arg1) {
    u8 *dst = arg0;
    u8 *src = arg1;

    while (*src != 0) {
        *dst = *src;
        src += 1;
        dst += 1;
    }
    *dst = 0;
    return dst;
}
