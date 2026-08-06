#include "global.h"
#include "types.h"

extern u8 D_030006A0;

u32 func_08005920(s32 arg0) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r3 = r0;
    if ((s32)r3 >= 0) {
        goto search;
    }
    goto done;
result_two:
    r0 = 2;
    goto return_value;
search:
    r4 = 0;
    r2 = (u32)&D_030006A0;
    r5 = 1;
    r6 = 0xFFFE;
loop:
    r0 = r5;
    r1 = *(u8 *)r2;
    r0 &= r1;
    if (r0 == 0) {
        goto next;
    }
    r0 = *(u32 *)(r2 + 8);
    if (r0 != r3) {
        goto next;
    }
    r1 = *(u16 *)r2;
    r0 = r6;
    r0 &= r1;
    if (r0 != 0) {
        goto result_two;
    }
    r0 = 1;
    goto return_value;
next:
    r4 += 1;
    r2 += 0x1C;
    if (r4 <= 0x2F) {
        goto loop;
    }
done:
    r0 = 0;
return_value:;
}
