#include "global.h"
#include "types.h"
#include "scenes.h"

void func_0801D4B4(u32 value) {
    register u8 *r1 asm("r1") = (u8 *)&gCurrentSceneVariable;
    asm volatile("" : "+r"(r1));
    r1 = *(u8 **)r1;
    r1 = *(u8 **)(r1 + 0xC);
    value <<= 8;
    asm volatile("" : "+r"(value), "+r"(r1));
    *(u16 *)r1 = value;
    value = 1;
    *(u8 *)(r1 + 3) = value;
}
