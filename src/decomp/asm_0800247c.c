#if __INCLUDE_LEVEL__ > 0
#include "global.h"

void func_0800247C(void *a0, void *a1) {
    register u32 *r2 asm("r2") = a0;
    register u32 *r1 asm("r1") = a1;
    register u32 r0 asm("r0");

    goto check;
    loop:
    r0 = *(u32 *)((u8 *)r1 + 0);
    *(u32 *)((u8 *)r2 + 0) = r0;
    r0 = *(u32 *)((u8 *)r1 + 4);
    *(u32 *)((u8 *)r2 + 4) = r0;
    r0 = *(u32 *)((u8 *)r1 + 8);
    *(u32 *)((u8 *)r2 + 8) = r0;
    r2 = (u32 *)((u8 *)r2 + 0xC);
    r1 = (u32 *)((u8 *)r1 + 0xC);
    check:
    r0 = *(u32 *)((u8 *)r1 + 0);
    if (r0 != 0) goto loop;
    r0 = 0;
    *(u32 *)((u8 *)r2 + 4) = r0;
    *(u32 *)((u8 *)r2 + 0) = r0;
    *(u32 *)((u8 *)r2 + 8) = r0;
}
#endif
