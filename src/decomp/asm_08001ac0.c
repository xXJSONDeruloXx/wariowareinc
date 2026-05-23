#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern u32 D_03000138;
extern u8 D_03000118[];
extern u32 D_03000140[];

s32 func_08001AC0(u32 arg0) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2") = 0;
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r1 = (u32)&D_03000138;
    r0 = *(u32 *)r1;
    if (r2 >= r0) goto not_found;
    r5 = (u32)D_03000118;
    r6 = 1;
    r3 = (u32)D_03000140;
    r4 = r1;
loop:
    r1 = r2 + r5;
    r0 = *(u8 *)r1;
    if (r0 != 0) goto next;
    *(u8 *)r1 = r6;
    *(u32 *)r3 = r0;
    r0 = r2;
    goto done;
next:
    r3 += 4;
    r2 += 1;
    r0 = *(u32 *)r4;
    if (r2 < r0) goto loop;
not_found:
    r0 = 1;
    r0 = -r0;
done:
    return r0;
}
#endif
