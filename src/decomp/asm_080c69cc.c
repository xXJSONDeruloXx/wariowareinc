#include "global.h"
#include "types.h"

void func_080C69CC(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2") = (u32)&gCurrentSceneVariable;

    r0 = *(u32 *)r2;
    r1 = 0x125;
    r0 += r1;
    r1 = 0;
    *(u8 *)r0 = r1;
    r0 = *(u32 *)r2;
    r2 = 0x93;
    r2 <<= 1;
    r0 += r2;
    *(u16 *)r0 = r1;
}
