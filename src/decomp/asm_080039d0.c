#include "global.h"
#include "types.h"

u32 func_080039D0(u8 **cursor) {
    register u8 *r2 asm("r2") = *cursor - 4;
    register u32 value asm("r0");
    register u32 temp asm("r1");
    *cursor = r2;
    value = r2[0];
    temp = r2[1] << 8;
    value |= temp;
    temp = r2[2] << 16;
    value |= temp;
    temp = r2[3] << 24;
    value |= temp;
    return value;
}
