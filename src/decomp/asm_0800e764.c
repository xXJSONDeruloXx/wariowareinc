#include "global.h"
#include "src/lib_sprite.h"
#include "scenes.h"

typedef struct {
    u8 pad0[0x2D0];
    s16 field2D0;
} Func0800E764SceneData;

void func_0800E764(void) {
    struct SpriteHandler *handler = gSpriteHandler;
    Func0800E764SceneData *scene = (Func0800E764SceneData *)gCurrentSceneData;
    s16 id = scene->field2D0;

    sprite_set_visible(handler, id, 0);
}
