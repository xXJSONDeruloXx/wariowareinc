#include "global.h"
#include "src/lib_sprite.h"

struct Func080B2450Scene {
    u8 pad0[0x1B4];
    s16 spriteId;
};

extern struct BeatscriptLocalData *gCurrentSceneVariable;

void func_080B2450(void) {
    struct Func080B2450Scene *scene;

    scene = (struct Func080B2450Scene *)gCurrentSceneVariable;
    sprite_set_visible(gSpriteHandler, scene->spriteId, 0);
}
