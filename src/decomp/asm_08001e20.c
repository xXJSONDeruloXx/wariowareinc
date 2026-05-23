#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 D_03000140[];
extern u8 D_03000118[];

u32 func_08001E20(u32 arg0) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4") = arg0;
    register u32 r5 asm("r5");

    r3 = 0;
    r1 = 0;
    r5 = (u32)D_03000118;
    r2 = (u32)D_03000140;
loop:
    r0 = r1 + r5;
    r0 = *(u8 *)r0;
    if (r0 == 0) goto next;
    r0 = *(u32 *)r2;
    if (r0 != r4) goto next;
    r3 += 1;
next:
    r2 += 4;
    r1 += 1;
    if (r1 <= 0x1F) goto loop;
    r0 = r3;
    return r0;
}
#endif
