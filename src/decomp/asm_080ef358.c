#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern u8 D_03000E70;
extern s32 sprite_is_invalid(void *, s16);
extern u32 __udivsi3(u32, u32);

typedef s32 (*Func080EF358SpriteInvalid)(void *, s32);
typedef u32 (*Func080EF358Udiv)(u32, u32);

u32 func_080EF358(void *arg0, s32 arg1) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1") = (u32)arg1;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r5 = r0;
    r2 = (u32)&D_03000E70;
    r0 = 0xB;
    *(u8 *)r2 = r0;
    r1 <<= 16;
    r4 = (u32)((s32)r1 >> 16);
    r0 = r5;
    r1 = r4;
    r0 = ((Func080EF358SpriteInvalid)sprite_is_invalid)((void *)r0, r1);
    if (r0 == 0) goto body;
    r0 = 0;
    goto done;
body:
    r0 = r4 << 3;
    r0 -= r4;
    r0 <<= 3;
    r1 = *(u32 *)(r5 + 8);
    r5 = r1 + r0;
    r1 = *(u32 *)(r5 + 8);
    r3 = 0;
    r4 = 0;
    r2 = 0xD;
    r2 = (u32)*(s8 *)(r5 + r2);
    if (r3 >= r2) goto after_loop;
loop:
    r0 = *(u8 *)(r1 + 4);
    r3 += r0;
    r1 += 8;
    r4 += 1;
    if (r4 < r2) goto loop;
after_loop:
    r0 = *(u8 *)(r1 + 4);
    r1 = *(u8 *)(r5 + 0xC);
    r0 -= r1;
    r3 += r0;
    r0 = r3 << 8;
    r1 = *(u16 *)(r5 + 0x24);
    r0 = ((Func080EF358Udiv)__udivsi3)(r0, r1);
    r0 <<= 24;
    r0 >>= 24;
done:
    return r0;
}
#endif
