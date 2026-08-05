#include "global.h"
#include "types.h"
#include "scenes.h"

void func_0801D4A0(u32 value) {
    register u8 *r1 asm("r1") = (u8 *)&gCurrentSceneVariable;
    register u8 *r2 asm("r2");
    asm volatile("" : "+r"(r1), "+r"(r2));
    r1 = *(u8 **)r1;
    r2 = *(u8 **)(r1 + 0xC);
    value <<= 8;
    r1 = 0;
    asm volatile("" : "+r"(value), "+r"(r1), "+r"(r2));
    *(u16 *)r2 = value;
    *(u8 *)(r2 + 3) = r1;
}
