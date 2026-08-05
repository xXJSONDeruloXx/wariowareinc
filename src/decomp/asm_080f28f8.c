#include "global.h"

u32 func_080F28F8(u32 value) {
    u32 r1 = value;
    register u32 r0 asm("r0");
    r1 >>= 3;
    r0 = r1;
    r0 >>= 1;
    r1 += r0;
    if (r1 > 0xF)
        r1 = 0xF;
    return r1;
}
