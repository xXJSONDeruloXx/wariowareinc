#include "global.h"
#include "types.h"

void func_08003998(u8 **cursor, u32 value) {
    register u8 *r2 asm("r2") = *cursor;
    register u32 r3 asm("r3");
    *r2++ = value;
    r3 = value >> 8;
    *r2++ = r3;
    r3 = value >> 16;
    *r2++ = r3;
    value >>= 24;
    *r2++ = value;
    *cursor = r2;
}
