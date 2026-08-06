#include "global.h"

extern u32 func_0800A024(void);

u32 func_080E1F48(void) {
    register u32 r0 asm("r0");
    register u32 r4 asm("r4") = 0x083E8448;

    asm("" : "+r"(r4));
    r0 = func_0800A024();
    r0 += r4;
    return *(u8 *)r0;
}
