#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void sprite_id_set_visible(struct SpriteHandler *, u32, u16);

void func_08016F98(void) {
    sprite_id_set_visible(gSpriteHandler, 1, 0);
}
