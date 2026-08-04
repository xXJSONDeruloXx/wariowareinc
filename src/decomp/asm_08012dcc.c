#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"
#include "scenes.h"

extern void sprite_set_visible(struct SpriteHandler *handler, s16 id, u16 isVisible);

void func_08012DCC(void) {
    u32 i;
    register u32 ids asm("r2");
    register u32 offset asm("r1");
    s16 id;
    struct SpriteHandler *handler;

    for (i = 0; i <= 0x1D; i++) {
        handler = gSpriteHandler;
        ids = *(u32 *)((u8 *)gCurrentSceneData + (0xEA << 1));
        offset = i << 1;
        offset += ids;
        id = *(s16 *)offset;
        sprite_set_visible(handler, id, 0);
    }
}
#endif
