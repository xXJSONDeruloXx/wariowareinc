#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

void func_080141C8(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");

    r3 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r3;
    r1 += 0xDE;
    r0 = *(u8 *)r1;
    r2 = 4;
    r0 |= r2;
    *(u8 *)r1 = r0;
    r0 = *(u32 *)r3;
    r0 += 0xFE;
    r4 = 0;
    r1 = 1;
    *(u8 *)r0 = r1;
    r1 = *(u32 *)r3;
    asm volatile("" ::: "r2");
    r2 = 0x80;
    r2 <<= 1;
    r0 = r1 + r2;
    *(u16 *)r0 = r2;
    asm volatile("" : "+r"(r2) : : "cc");
    r2 += 2;
    r0 = r1 + r2;
    *(u16 *)r0 = r4;
    r0 = 0x82;
    r0 <<= 1;
    r1 += r0;
    r0 = 0xA0;
    *(u16 *)r1 = r0;
}
#endif
