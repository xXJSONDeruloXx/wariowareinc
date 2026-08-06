#include "global.h"
#include "src/lib_sprite.h"

extern void scene_set_current_thread(u32);

void func_080EC55C(void) {
    scene_set_current_thread(1);
    sprite_set_visible(gSpriteHandler,
                       *(s16 *)((u8 *)gCurrentSceneVariable + 6),
                       0);
}
