#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern u32 D_03000010[];
extern u8 D_03000118[];

void func_08001B28(s32 arg0) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6") = (u32)arg0;

    if ((s32)r6 < 0) goto done;
    asm volatile("" : "+r"(r6));
    r2 = (u32)D_03000010;
    r0 = r6 << 3;
    r0 += r2;
    r5 = 0;
    r3 = 0;
    r4 = 0x80;
    r4 <<= 1;
    *(u16 *)r0 = r4;
    r1 = r6 << 2;
    r0 = r1 + 1;
    r0 <<= 1;
    r0 += r2;
    *(u16 *)r0 = r3;
    r0 = r1 + 2;
    r0 <<= 1;
    r0 += r2;
    *(u16 *)r0 = r3;
    r1 += 3;
    r1 <<= 1;
    r1 += r2;
    *(u16 *)r1 = r4;
    r0 = (u32)D_03000118;
    asm volatile("" : "+r"(r6), "+r"(r0));
    r0 = r6 + r0;
    *(u8 *)r0 = r5;
done:;
}
#endif
