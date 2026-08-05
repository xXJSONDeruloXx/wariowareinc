#include "global.h"
#include "src/lib_sprite.h"

extern void sprite_id_set_visible(struct SpriteHandler *, u32, u16);

void func_080C61AC(void) {
    sprite_id_set_visible(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + 0x128), 1);
}
