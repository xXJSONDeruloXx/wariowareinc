#include "global.h"
#include "src/lib_sprite.h"

struct Func08040AACScene {
    u8 pad0[0x5E];
    s16 spriteId;
};

extern struct BeatscriptLocalData *gCurrentSceneVariable;

void func_08040AAC(void) {
    struct Func08040AACScene *scene;

    scene = (struct Func08040AACScene *)gCurrentSceneVariable;
    sprite_set_anim_cel(gSpriteHandler, scene->spriteId, 0);
}
