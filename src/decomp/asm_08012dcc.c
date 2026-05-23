#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"
#include "scenes.h"

extern void sprite_set_visible(struct SpriteHandler *handler, s16 id, u16 isVisible);

void func_08012DCC(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4") = 0;

loop:
    r0 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r0;
    r1 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r1;
    r2 = 0xEA;
    r2 <<= 1;
    r1 += r2;
    r2 = *(u32 *)r1;
    r1 = r4 << 1;
    r1 += r2;
    r2 = 0;
    r1 = *(s16 *)(r1 + r2);
    r2 = 0;
    asm volatile("bl sprite_set_visible" :: "r"(r0), "r"(r1), "r"(r2) : "r0", "r1", "r2", "r3", "lr", "memory");
    r4 += 1;
    if (r4 <= 0x1D) goto loop;
}
#endif
