#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"
#include "scenes.h"

void func_080164CC(void) {
    u8 *scene;
    u32 value;
    s16 id;

    scene = (u8 *)gCurrentSceneData;
    value = *(u16 *)(scene + 0x1BA);
    value -= 1;
    *(u16 *)(scene + 0x1BA) = value;
    if ((value << 16) == 0) {
        *(u8 *)(scene + 0x1B8) = 2;
        id = *(s16 *)((u8 *)gCurrentSceneSpritePool + 0x32);
        sprite_set_visible(gSpriteHandler, id, 1);
        sprite_set_anim_cel(gSpriteHandler, id, 0);
    }
}
