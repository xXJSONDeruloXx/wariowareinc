#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"

extern void sprite_id_set_visible(struct SpriteHandler *, u32, u16);

void func_080733AC(void) {
    sprite_id_set_visible(gSpriteHandler,
                          *(u32 *)((u8 *)gCurrentSceneData + (0xF8 << 1)),
                          0);
}
