#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/lib_sprite.h"

u32 sprite_get_anim_duration(struct Animation *anim) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1") = (u32)anim;
    register u32 r2 asm("r2");

    r2 = 0;
    goto check;
loop:
    r0 = *(u8 *)(r1 + 4);
    r0 = r2 + r0;
    r0 <<= 16;
    r2 = r0 >> 16;
    r1 += 8;
check:
    r0 = *(u32 *)r1;
    if (r0 != 0) goto loop;
    r0 = r2;
    return r0;
}
#endif
