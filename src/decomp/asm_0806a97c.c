#include "global.h"
#include "src/lib_sprite.h"

extern void scene_set_current_thread(u32);

void func_0806A97C(struct SpriteHandler *arg0, s32 arg1, void *arg2) {
    scene_set_current_thread(1);
    sprite_set_visible(arg0, *(s16 *)arg2, 0);
    *(u32 *)((u8 *)arg2 + 4) = 0;
}
