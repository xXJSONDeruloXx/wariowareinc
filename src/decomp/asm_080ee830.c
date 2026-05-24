#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern s32 func_080efc88(void *);

s32 func_080EE830(void *arg0) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2") = (u32)arg0;
    register u32 r3 asm("r3");
    s32 (*func)(void *);

    r1 = *(u32 *)r2;
    r0 = *(u16 *)r1;
    *(u8 *)(r2 + 9) = r0;
    r0 <<= 24;
    if (r0 != 0) goto nonzero;
    r0 = 0;
    goto done;
nonzero:
    r3 = r1 + 2;
    *(u32 *)r2 = r3;
    r0 = 0;
    *(u8 *)(r2 + 8) = r0;
    r0 = *(s8 *)(r2 + 0xC);
    if ((s32)r0 >= 0) goto call;
    r1 = *(u8 *)(r2 + 9);
    r1 -= 1;
    r0 = r1 << 1;
    r0 += r1;
    r0 <<= 1;
    r0 = r3 + r0;
    *(u32 *)r2 = r0;
call:
    func = func_080efc88;
    r0 = (u32)func((void *)r2);
done:
    return r0;
}
#endif
