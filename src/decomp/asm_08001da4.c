#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern u32 D_03000138;
extern u32 D_03000110;
extern u32 D_03000010[];

void func_08001DA4(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2") = 0;
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r4 = (u32)&D_03000138;
    r0 = *(u32 *)r4;
    r0 <<= 2;
    if (r2 >= r0) goto done;
    r5 = (u32)&D_03000110;
    r3 = (u32)D_03000010;
loop:
    r1 = *(u32 *)r5;
    r0 = r2 << 3;
    r0 += r1;
    r1 = *(u16 *)r3;
    *(u16 *)(r0 + 6) = r1;
    r3 += 2;
    r2 += 1;
    r0 = *(u32 *)r4;
    r0 <<= 2;
    if (r2 < r0) goto loop;
done:;
}
#endif
