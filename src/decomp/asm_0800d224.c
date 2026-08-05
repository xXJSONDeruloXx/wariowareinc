#include "global.h"
#include "types.h"
#include "scenes.h"

void func_0800D224(u32 index, u32 value) {
    register u8 *r2 asm("r2") = (u8 *)&gBeatscriptScene;
    register u32 r3 asm("r3");
    index <<= 2;
    asm volatile("" : "+r"(index), "+r"(r2));
    r3 = 0x1C5C;
    asm volatile("" : "+r"(r3));
    r2 += r3;
    index += (u32)r2;
    *(u32 *)index = value;
}
