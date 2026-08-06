#include "global.h"
#include "types.h"

extern u8 D_030006A0;

void func_080059A8(s32 arg0, u32 arg1) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1") = arg1;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r4 = (u32)arg0;
    r1 <<= 16;
    r5 = r1 >> 16;
    if ((s32)r4 < 0) {
        goto done;
    }
    r3 = 0;
    r2 = (u32)&D_030006A0;
    r6 = 1;
loop:
    r1 = *(u8 *)r2;
    r0 = r6;
    r0 &= r1;
    if (r0 == 0) {
        goto next;
    }
    r0 = *(u32 *)(r2 + 8);
    if (r0 != r4) {
        goto next;
    }
    *(u16 *)(r2 + 0x18) = r5;
    goto done;
next:
    r3 += 1;
    r2 += 0x1C;
    if (r3 <= 0x2F) {
        goto loop;
    }
done:;
}
