#include "global.h"

void func_08003998(u8 **cursor, u32 value) {
    u8 *p = *cursor;
    *p++ = value;
    *p++ = value >> 8;
    *p++ = value >> 16;
    *p++ = value >> 24;
    *cursor = p;
}
