#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 func_0801274C(u32);
extern u8 D_083AA0C4[];

s32 func_08012798(u32 arg0) {
    register u32 r0 asm("r0") = arg0;
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r5 = (u32)D_083AA0C4;
    r0 <<= 4;
    goto check;
body:
    r0 = r4;
    r0 = func_0801274C(r0);
    if (r0 != 0) { r0 = r4; goto done; }
    r0 = r4;
    r0 <<= 4;
check:
    r0 += r5;
    r4 = 5;
    r4 = *(s8 *)(r0 + r4);
    if ((s32)r4 >= 0) goto body;
    r0 = 1;
    r0 = -r0;
done:
    return r0;
}
#endif
