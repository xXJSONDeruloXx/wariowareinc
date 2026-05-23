#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

s32 func_080EF31C(void *handler, s16 id) {
    register u32 r0 asm("r0") = (u32)handler;
    register u32 r1 asm("r1") = (u32)id;
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r5 = r0;
    asm volatile("" ::: "r1");
    r2 = (u32)&D_03000E70;
    r0 = 0xA;
    *(u8 *)r2 = r0;
    r1 <<= 16;
    r4 = (u32)((s32)r1 >> 16);
    r0 = r5;
    r1 = r4;
    r0 = sprite_is_invalid((void *)r0, r1);
    if (r0 != 0) goto invalid;
    r0 = *(u32 *)(r5 + 8);
    r1 = r4 << 3;
    r1 -= r4;
    r1 <<= 3;
    r1 += r0;
    r0 = 0xD;
    r0 = *(s8 *)(r1 + r0);
    goto done;
invalid:
    r0 = 1;
    r0 = -r0;
done:
    return r0;
}
__attribute__((section(".text"))) const u16 _padding_080ef31c = 0;
#endif
