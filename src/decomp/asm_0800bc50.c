#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void sprite_set_visible(struct SpriteHandler *handler, s16 id, u16 isVisible);

typedef void (*Func0800BC50SetVisible)(struct SpriteHandler *, s32, s32);

void func_0800BC50(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");

    r4 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r4;
    r2 = 0xC0;
    r2 <<= 1;
    r0 = r1 + r2;
    r0 = *(u32 *)r0;
    if (r0 == 0) goto done;
    r0 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r0;
    r2 += 8;
    r1 = r1 + r2;
    r2 = 0;
    r1 = *(s16 *)(r1 + r2);
    r2 = 0;
    ((Func0800BC50SetVisible)sprite_set_visible)((struct SpriteHandler *)r0, r1, r2);
    r0 = *(u32 *)r4;
    r2 = 0x195;
    r1 = r0 + r2;
    r0 = 4;
    *(u8 *)r1 = r0;
done:;
}
#endif
