#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"
#include "scenes.h"

extern void sprite_set_visible(struct SpriteHandler *handler, s16 id, u16 isVisible);

struct Func08012DCCScene {
    u8 padding[0x1D4];
    s16 *ids;
};

void func_08012DCC(void) {
    u32 i;
    s16 id;
    struct SpriteHandler *handler;
    struct Func08012DCCScene *scene;
    s16 *ids;

    for (i = 0; i <= 0x1D; i++) {
        handler = gSpriteHandler;
        scene = (struct Func08012DCCScene *)gCurrentSceneData;
        ids = scene->ids;
        id = ids[i];
        sprite_set_visible(handler, id, 0);
    }
}
#endif
