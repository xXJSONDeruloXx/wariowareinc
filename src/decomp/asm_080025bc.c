#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void dma3_set(const void *source, void *destination, u32 bytesToSet, u16 unit, u32 bytesPerInterrupt);

void func_080025BC(u32 arg0, u32 arg1) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1") = arg1;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    u32 sp[1];

    r5 = r0;
    r4 = r1;
    goto check;
loop:
    r0 = *(u32 *)r4;
    r1 = *(u32 *)(r4 + 4);
    r1 <<= 1;
    r1 = r5 + r1;
    r2 = *(u32 *)(r4 + 8);
    r3 = 0x80;
    r3 <<= 1;
    sp[0] = r3;
    r3 = 0x20;
    dma3_set((const void *)r0, (void *)r1, r2, r3, sp[0]);
    r4 += 0xC;
check:
    r0 = *(u32 *)r4;
    if (r0 != 0) goto loop;
    r0 = *(u32 *)(r4 + 4);
    if (r0 != 0) goto loop;
    r0 = *(u32 *)(r4 + 8);
    if (r0 != 0) goto loop;
}
#endif
