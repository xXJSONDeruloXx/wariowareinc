#include "global.h"
#include "types.h"

extern u8 D_030006A0;
extern void task_stop(void *, s32);

void func_08005834(s32 arg0) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r3 = r0;
    if ((s32)r3 < 0) {
        goto done;
    }
    r1 = 0;
    r2 = (u32)&D_030006A0;
    goto scan;
next:
    r1 += 1;
    r2 += 0x1C;
    if (r1 > 0x2F) {
        goto done;
    }
scan:
    r0 = *(u32 *)(r2 + 8);
    if (r0 != r3) {
        goto next;
    }
    if (r1 > 0x2F) {
        goto done;
    }
    r1 = *(u8 *)r2;
    r0 = 1;
    r0 &= r1;
    if (r0 == 0) {
        goto done;
    }
    task_stop((void *)r2, 0);
done:;
}
