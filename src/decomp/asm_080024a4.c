#if __INCLUDE_LEVEL__ > 0
#include "global.h"

void func_080024A4(void *dest, void *src, s32 count) {
    register u32 *r3 asm("r3") = dest;
    register const u32 *r1 asm("r1") = src;
    register s32 r2 asm("r2") = count;
    register u32 r0 asm("r0");

    goto check;
    loop:
    *(u32 *)((u8 *)r3 + 0) = r0;
    r0 = *(u32 *)((u8 *)r1 + 4);
    *(u32 *)((u8 *)r3 + 4) = r0;
    r0 = *(u32 *)((u8 *)r1 + 8);
    *(u32 *)((u8 *)r3 + 8) = r0;
    r3 = (u32 *)((u8 *)r3 + 0xC);
    r1 = (const u32 *)((u8 *)r1 + 0xC);
    r2--;
    check:
    r0 = *(u32 *)((u8 *)r1 + 0);
    if (r0 == 0) goto done;
    if (r2 != 0) goto loop;
    done:
    r0 = 0;
    *(u32 *)((u8 *)r3 + 4) = r0;
    *(u32 *)((u8 *)r3 + 0) = r0;
    *(u32 *)((u8 *)r3 + 8) = r0;
}
#endif
