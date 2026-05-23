#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

void func_0800898C(void *arg0, u32 arg1, u32 arg2, void *arg3) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1") = arg1;
    register u32 r2 asm("r2") = arg2;
    register u32 r3 asm("r3") = (u32)arg3;
    register u32 r4 asm("r4");
    register u32 r5 asm("r5") = (u32)arg0;
    register u32 r6 asm("r6") = (u32)arg3;

    r4 = *(u32 *)(r5 + 4);
    goto start;
loop:
    r4 += 8;
start:
    r0 = *(u8 *)r4;
    if (r0 != 0xFF) goto loop;
    r3 = 0;
    *(u8 *)r4 = r1;
    r1 = 0x3FF;
    r1 &= r2;
    r1 <<= 8;
    r0 = *(u32 *)r4;
    r2 = 0xFFFC00FF;
    r0 &= r2;
    r0 |= r1;
    *(u32 *)r4 = r0;
    *(u32 *)(r4 + 4) = r6;
    r4 += 8;
    r0 = 0xFF;
    *(u8 *)r4 = r0;
    r0 = *(u32 *)r4;
    r0 &= r2;
    *(u32 *)r4 = r0;
    *(u32 *)(r4 + 4) = r3;
    r0 = *(u8 *)r5;
    r0 += 1;
    *(u8 *)r5 = r0;
}
#endif
