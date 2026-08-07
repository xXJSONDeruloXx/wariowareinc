#include "global.h"
#include "src/lib_sprite.h"

void func_08017054(u32 arg0) {
    s16 sprite_id;

    sprite_id = (s16)arg0;
    sprite_set_base_tile(gSpriteHandler, sprite_id, 0x300);
    sprite_set_base_palette(gSpriteHandler, sprite_id, 0xC);
}
