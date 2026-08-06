#include "global.h"
#include "types.h"

extern u8 D_030006A0;
extern void task_stop(void *, s32);

void func_080058AC(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r5 = 0;
    r4 = (u32)&D_030006A0;
loop:
    r1 = *(u8 *)r4;
    r0 = 1;
    r0 &= r1;
    if (r0 == 0) {
        goto next;
    }
    r0 = *(u32 *)(r4 + 8);
    if ((s32)r0 < 0) {
        goto next;
    }
    task_stop((void *)r4, 1);
next:
    r5 += 1;
    r4 += 0x1C;
    if (r5 <= 0x2F) {
        goto loop;
    }
}
