#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"

extern s32 func_080049BC(s32, s32);
extern s16 sprite_create(struct SpriteHandler *, s32, s32, s16, s32, s32,
                         s32, s32, s32);

struct NameSelectSceneData {
    u8 pad0[8];
    s32 unk8;
};

s16 func_0800DE84(s32 arg0, u16 arg1, u16 arg2) {
    struct NameSelectSceneData *scene;
    s16 id;

    scene = (struct NameSelectSceneData *)gCurrentSceneData;
    id = sprite_create(gSpriteHandler, func_080049BC(scene->unk8, arg0), 0,
                       (s16)arg1, (s32)(s16)arg2, 0x800, 0, 0, 0);
    sprite_set_visible(gSpriteHandler, id, 0);
    return id;
}
#endif
