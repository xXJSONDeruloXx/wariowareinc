#if __INCLUDE_LEVEL__ > 0
#include "global.h"

struct Sprite;
struct SpriteHandler;

s16 sprite_handler_alloc_id(struct SpriteHandler *handler) {
    register struct SpriteHandler *r3 asm("r3");
    register u16 r4 asm("r4");
    register s32 r2 asm("r2");
    register s32 r0 asm("r0");
    register struct Sprite *sprites asm("r0");
    register s32 r1 asm("r1");
    register s32 negCheck asm("r0");
    register u32 sentinel asm("r0");

    r3 = handler;
    r4 = *(u16 *)((u8 *)r3 + 0x10);
    r2 = *(s16 *)((u8 *)r3 + 0x10);
    if (r2 < 0) goto done;
    sprites = *(struct Sprite **)((u8 *)r3 + 8);
    r1 = r2 << 3;
    r1 -= r2;
    r1 <<= 3;
    r1 += (s32)sprites;
    negCheck = *(u16 *)((u8 *)r1 + 0x1A);
    *(u16 *)((u8 *)r3 + 0x10) = negCheck;
    negCheck <<= 16;
    if (negCheck >= 0) goto done;
    sentinel = 0x0000FFFF;
    *(u16 *)((u8 *)r3 + 0x12) = (u16)sentinel;
    done:
    return (s16)r4;
}
#endif
