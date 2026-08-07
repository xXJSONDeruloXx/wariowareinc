#include "global.h"
#include "src/lib_sprite.h"

struct Func080D70ECScene {
    u8 pad0[8];
    s16 spriteId;
};

extern struct BeatscriptLocalData *gCurrentSceneVariable;
extern s32 func_080EF31C(void *handler, s16 id);

s8 func_080D70EC(void) {
    struct Func080D70ECScene *scene;

    scene = (struct Func080D70ECScene *)gCurrentSceneVariable;
    return (s8)func_080EF31C(gSpriteHandler, scene->spriteId);
}
