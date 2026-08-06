#include "global.h"
#include "types.h"

extern u8 D_030006A0;

void func_08005A54(u32 arg0, s32 arg1) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1") = (u32)arg1;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r0 <<= 16;
    r6 = r0 >> 16;
    r3 = 0;
    r2 = (u32)&D_030006A0;
    r4 = 1;
    r5 = r1 << 1;
loop:
    r1 = *(u8 *)r2;
    r0 = r4;
    r0 &= r1;
    if (r0 == 0) {
        goto next;
    }
    r0 = *(u16 *)(r2 + 2);
    if (r0 != r6) {
        goto next;
    }
    r1 = *(u16 *)r2;
    r0 = r4;
    r0 &= r1;
    r0 |= r5;
    *(u16 *)r2 = r0;
next:
    r3 += 1;
    r2 += 0x1C;
    if (r3 <= 0x2F) {
        goto loop;
    }
}
