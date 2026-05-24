#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void *gCurrentSceneData;

void func_08014E38(void) {
    register u32 r4 asm("r4") = 0;
    register u32 r5 asm("r5");
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    u8 count;

    r1 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r1;
    r2 = 0xC8;
    r2 <<= 1;
    r0 = r0 + r2;
    count = *(u8 *)r0;

    if (r4 >= (u32)count) goto done;

    r5 = r1;

loop:
    r0 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r0;
    r1 = *(u32 *)r5;
    r2 = 0xCA;
    r2 <<= 1;
    r1 = r1 + r2;
    r2 = *(u32 *)r1;
    r1 = r4 << 1;
    r1 = r1 + r2;
    r2 = 2;
    r1 = *(s16 *)(r1 + r2);
    r2 = 6;
    asm volatile("bl sprite_set_base_palette" :: "r"(r0), "r"(r1), "r"(r2) : "r0", "r1", "r2", "r3", "lr", "memory");

    r4 += 1;
    r0 = *(u32 *)r5;
    r1 = 0xC8;
    r1 <<= 1;
    r0 = r0 + r1;
    r0 = *(u8 *)r0;
    if ((u32)r4 < (u32)r0) goto loop;

done:
    return;
}
#endif
