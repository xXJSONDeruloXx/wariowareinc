#include "global.h"
#include "src/lib_sprite.h"

extern void scene_set_current_thread(u32);

void func_080EC62C(void) {
    scene_set_current_thread(1);
    sprite_set_visible(gSpriteHandler,
                       *(s16 *)((u8 *)gCurrentSceneVariable + 0xE),
                       0);
}
