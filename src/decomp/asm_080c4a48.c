#include "global.h"
#include "types.h"

void func_080C4A48(u32 value) {
    register u8 *r1 asm("r1") = (u8 *)gCurrentSceneVariable;
    register s32 r0 asm("r0") = (s16)value;
    register u32 r2 asm("r2");
    asm volatile("" : "+r"(r0), "+r"(r1));
    r2 = *(u16 *)(r1 + 8);
    r0 += r2;
    *(u16 *)(r1 + 8) = r0;
}
