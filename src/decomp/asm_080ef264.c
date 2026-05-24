#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void sprite_set_x(void *handler, s32 id, s32 x) {
    register u32 r0 asm("r0") = (u32)handler;
    register u32 r1 asm("r1") = (u32)id;
    register u32 r2 asm("r2") = (u32)x;
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r5 = r0;
    asm volatile("" : "+r"(r0), "+r"(r5));
    r2 <<= 16;
    r6 = r2 >> 16;
    r2 = (u32)&D_03000E70;
    r0 = 7;
    *(u8 *)r2 = r0;
    r1 <<= 16;
    r4 = (u32)((s32)r1 >> 16);
    r0 = r5;
    r1 = r4;
    r0 = sprite_is_invalid((void *)r0, r1);
    if (r0 != 0) goto done;
    r1 = *(u32 *)(r5 + 8);
    r0 = r4 << 3;
    r0 -= r4;
    r0 <<= 3;
    r0 += r1;
    *(u16 *)(r0 + 2) = r6;
done:;
}
#endif
