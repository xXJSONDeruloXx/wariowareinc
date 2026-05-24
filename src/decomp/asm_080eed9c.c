#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/lib_sprite.h"

u32 sprite_anim_get_cel_total(struct Animation *anim) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2") = (u32)anim;

    r1 = 0;
    r0 = *(u32 *)r2;
    if (r0 == 0) goto done;
loop:
    r0 = r1 + 1;
    r0 <<= 24;
    r1 = r0 >> 24;
    r0 = r1 << 3;
    r0 += r2;
    r0 = *(u32 *)r0;
    if (r0 != 0) goto loop;
done:
    r0 = r1;
    return r0;
}
#endif
