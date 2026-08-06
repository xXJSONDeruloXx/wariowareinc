#include "global.h"
#include "types.h"

void func_0800C9C0(void) {
    register u8 *base asm("r1") = (u8 *)&gBeatscriptScene;
    register u32 value asm("r2");
    register u32 offset asm("r0");

    value = base[1];
    offset = 0x11;
    offset = -offset;
    offset &= value;
    base[1] = offset;
    offset = 0x1C30;
    value = (u32)base + offset;
    offset = 0;
    *(u16 *)value = offset;
    offset = 0x1C32;
    value = (u32)base + offset;
    offset = 0x18;
    *(u16 *)value = offset;
}
