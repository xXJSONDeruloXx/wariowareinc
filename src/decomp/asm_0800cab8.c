#include "global.h"
#include "types.h"

void func_0800CAB8(u32 value) {
    register u8 *r1 asm("r1") = (u8 *)&gBeatscriptScene;
    register u32 r2 asm("r2") = 0x1C30;
    asm volatile("" : "+r"(r1), "+r"(r2));
    *(u16 *)(r1 + r2) = value;
}
