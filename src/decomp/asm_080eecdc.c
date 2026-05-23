#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void sprite_remove_z_link(void *handler, s16 id) {
    register u32 r0 asm("r0") = (u32)handler;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r4 = r0;
    r3 = *(u32 *)(r4 + 8);
    r1 = (u32)id;
    r1 <<= 16;
    r1 = (s32)r1 >> 16;
    r0 = r1 << 3;
    r0 -= r1;
    r0 <<= 3;
    r0 += r3;
    r2 = *(u16 *)(r0 + 0x1A);
    r5 = *(u16 *)(r0 + 0x18);
    r6 = 0x18;
    r1 = *(s16 *)(r0 + r6);
    if ((s32)r1 < 0) goto store_head;
    r0 = r1 << 3;
    r0 -= r1;
    r0 <<= 3;
    r0 += r3;
    *(u16 *)(r0 + 0x1A) = r2;
    goto after_prev;
store_head:
    *(u16 *)(r4 + 0xC) = r2;
after_prev:
    r0 = r2 << 16;
    r1 = (s32)r0 >> 16;
    if ((s32)r1 < 0) goto store_tail;
    r0 = r1 << 3;
    r0 -= r1;
    r0 <<= 3;
    r0 += r3;
    *(u16 *)(r0 + 0x18) = r5;
    goto after_next;
store_tail:
    *(u16 *)(r4 + 0xE) = r5;
after_next:;
}
#endif
