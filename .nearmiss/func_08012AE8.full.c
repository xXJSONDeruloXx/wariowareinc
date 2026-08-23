#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"

struct Func08012AE8Scene {
    u8 pad0[0x9C];
    s16 spriteIds[1];
};

void func_08012AE8(u32 arg0) {
    struct Func08012AE8Scene *scene;
    u32 value;
    u32 index;
    s16 spriteId;

    value = arg0;
    index = 0;
    while (1) {
        scene = (struct Func08012AE8Scene *)gCurrentSceneData;
        spriteId = scene->spriteIds[index];
        sprite_set_anim_cel(gSpriteHandler, spriteId, (s8)(value % 10));
        sprite_set_visible(gSpriteHandler, spriteId, 1);
        value /= 10;
        if (value == 0) {
            break;
        }
        index += 1;
        if (index > 7) {
            break;
        }
    }
}
