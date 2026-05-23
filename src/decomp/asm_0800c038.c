#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "graphics.h"
#include "types.h"

void func_0800C038(s16 arg0, s16 arg1) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1") = (u32)arg1;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");

    r0 <<= 16;
    r1 <<= 16;
    r3 = r1 >> 16;
    r4 = r0 >> 16;
    if ((s32)r0 < 0) goto skip_x;
    r0 = (u32)&gGraphicsBuffer;
    r0 += 0x48;
    r2 = *(u16 *)r0;
    r1 = 0xFFF0;
    r1 &= r2;
    r1 |= r4;
    *(u16 *)r0 = r1;
skip_x:
    r0 = r3 << 16;
    r3 = (s32)r0 >> 16;
    if ((s32)r3 < 0) goto done;
    r2 = (u32)&gGraphicsBuffer;
    r2 += 0x48;
    r1 = *(u16 *)r2;
    r0 = 0xFF0F;
    r0 &= r1;
    r1 = r3 << 4;
    r0 |= r1;
    *(u16 *)r2 = r0;
done:;
}
#endif
