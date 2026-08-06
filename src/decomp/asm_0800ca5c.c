#include "global.h"
#include "types.h"

void func_0800CA5C(void) {
    register u8 *base asm("r2") = (u8 *)&gBeatscriptScene;
    register u32 offset asm("r0") = 0x1C34;
    register u32 addr asm("r1");

    addr = (u32)base + offset;
    offset = 0;
    *(u16 *)addr = offset;
    offset = base[1];
    addr = 0x10;
    offset |= addr;
    addr = 0x21;
    asm("" : "+r"(addr));
    addr = -addr;
    offset &= addr;
    base[1] = offset;
}
