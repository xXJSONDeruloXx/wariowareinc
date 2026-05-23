#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

void func_08015A4C(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r0 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r0;
    r0 = *(u32 *)(r1 + 0xC);
    r3 = 0x90;
    r3 <<= 2;
    r2 = r0 + r3;
    r0 = r1;
    r0 += 0xB4;
    r0 = *(u8 *)r0;
    if (r0 == 0) goto zero;
    r0 = r1;
    r0 += 0xC2;
    r1 = *(u16 *)r0;
    r0 = 0x80;
    r0 <<= 0xD;
    r1 |= r0;
    goto store;
zero:
    r1 = 0;
store:
    r0 = 0;
loop:
    asm volatile("stm %2!, {%1}" : "+r"(r2) : "r"(r1), "r"(r2) : "memory");
    r0 += 1;
    if (r0 <= 0xF) goto loop;
}
#endif
