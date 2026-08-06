#include "global.h"
#include "types.h"

void func_08005B40(void *arg0, s32 arg1, s32 arg2, u16 arg3) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1") = (u32)arg1;
    register u32 r2 asm("r2") = (u32)arg2;
    register u32 r3 asm("r3") = (u32)arg3;
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r4 = (u32)arg0 + 0;
    asm("" : "+r"(r4), "+r"(r3));
    r3 <<= 16;
    r3 >>= 16;
    if ((s32)r1 < 0) {
        goto done;
    }
    r0 = *(u16 *)(r4 + 4);
    if ((s32)r1 >= (s32)r0) {
        goto done;
    }
    if ((s32)r2 < 0) {
        goto done;
    }
    r5 = *(u16 *)(r4 + 6);
    if ((s32)r2 >= (s32)r5) {
        goto done;
    }
    r0 *= r2;
    r0 = r1 + r0;
    r1 = *(u32 *)r4;
    r0 <<= 1;
    r0 += r1;
    *(u16 *)r0 = r3;
done:;
}
