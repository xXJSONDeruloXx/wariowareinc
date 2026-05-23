#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern u32 D_03000110;
extern u32 D_03000138;
extern u32 D_03000010[];
extern u8 D_03000118[];

void func_08001A70(u32 arg0, u32 arg1) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1") = arg1;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4") = arg0;
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r0 = (u32)&D_03000110;
    *(u32 *)r0 = r1;
    r0 = (u32)&D_03000138;
    *(u32 *)r0 = r4;
    r1 = (u32)D_03000010;
    r2 = 0;
    asm volatile("" : "+r"(r0), "+r"(r1), "+r"(r2));
    if (r2 >= r4) goto done;
    r0 = 0x80;
    r0 <<= 1;
    r5 = r0;
    asm volatile("" : "+r"(r0), "+r"(r5));
    r3 = 0;
    asm volatile("" : "+r"(r3));
    r6 = (u32)D_03000118;
loop:
    *(u16 *)r1 = r5;
    r1 += 2;
    *(u16 *)r1 = r3;
    r1 += 2;
    *(u16 *)r1 = r3;
    r1 += 2;
    *(u16 *)r1 = r5;
    r1 += 2;
    r0 = r2 + r6;
    *(u8 *)r0 = r3;
    r2 += 1;
    if (r2 < r4) goto loop;
done:;
}
#endif
