#include "global.h"
#include "types.h"

void func_0801B174(u16 arg0) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");

    r0 <<= 16;
    r0 >>= 16;
    asm("" ::: "memory");
    r1 = (u32)&gCurrentSceneVariable;
    r2 = *(u32 *)r1;
    r1 = r2;
    r1 += 0xF0;
    *(u16 *)r1 = r0;
    r1 -= 2;
    *(u16 *)r1 = r0;
    r0 = *(u8 *)(r2 + 0x19);
    r1 = 2;
    r0 |= r1;
    *(u8 *)(r2 + 0x19) = r0;
}
