#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void sprite_handler_dealloc_id(void *handler, s16 id) {
    register u32 r0 asm("r0") = (u32)handler;
    register u32 r1 asm("r1") = (u32)id;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3") = (u32)handler;
    register u32 r4 asm("r4");

    asm volatile("" ::: "r1");
    r1 <<= 16;
    r4 = r1 >> 16;
    if ((s32)r1 < 0) goto done;
    r1 = 0x12;
    r0 = *(s16 *)(r3 + r1);
    if ((s32)r0 < 0) goto store_direct;
    r2 = *(u32 *)(r3 + 8);
    r1 = r0 << 3;
    r1 -= r0;
    r1 <<= 3;
    r1 += r2;
    *(u16 *)(r1 + 0x1A) = r4;
    goto after;
store_direct:
    *(u16 *)(r3 + 0x10) = r4;
after:
    r1 = r4 << 16;
    r1 = (s32)r1 >> 16;
    r2 = *(u32 *)(r3 + 8);
    r0 = r1 << 3;
    r0 -= r1;
    r0 <<= 3;
    r0 += r2;
    r1 = 0xFFFF;
    *(u16 *)(r0 + 0x1A) = r1;
    *(u16 *)(r3 + 0x12) = r4;
done:;
}
#endif
