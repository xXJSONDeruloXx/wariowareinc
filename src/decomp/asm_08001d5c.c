#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 D_03000010[];

void func_08001D5C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1") = (u32)arg1;
    register u32 r2 asm("r2") = (u32)arg2;
    register u32 r3 asm("r3") = (u32)arg3;
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r4 = r0;
    r0 = (u32)arg4;
    r1 <<= 16;
    r1 >>= 16;
    r2 <<= 16;
    r5 = r2 >> 16;
    r3 <<= 16;
    r3 >>= 16;
    r0 <<= 16;
    r6 = r0 >> 16;
    if ((s32)r4 < 0) goto done;
    r2 = (u32)D_03000010;
    r0 = r4 << 3;
    r0 += r2;
    *(u16 *)r0 = r1;
    r1 = r4 << 2;
    r0 = r1 + 1;
    r0 <<= 1;
    r0 += r2;
    *(u16 *)r0 = r5;
    r0 = r1 + 2;
    r0 <<= 1;
    r0 += r2;
    *(u16 *)r0 = r3;
    r1 += 3;
    r1 <<= 1;
    r1 += r2;
    *(u16 *)r1 = r6;
done:;
}
#endif
