#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_08001B28(s32);
extern u32 D_03000140[];
extern u8 D_03000118[];

void func_08001B70(u32 arg0) {
    register u32 r0 asm("r0") = arg0;
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r6 = r0;
    r4 = 0;
    r5 = (u32)D_03000140;
loop:
    r0 = (u32)D_03000118;
    r0 = r4 + r0;
    r0 = *(u8 *)r0;
    if (r0 == 0) goto next;
    r0 = *(u32 *)r5;
    if (r0 != r6) goto next;
    r0 = r4;
    func_08001B28(r0);
next:
    r5 += 4;
    r4 += 1;
    if ((s32)r4 <= 0x1F) goto loop;
}
#endif
