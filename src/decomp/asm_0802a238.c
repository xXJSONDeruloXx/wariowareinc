#include "global.h"
#include "src/lib_sprite.h"

void func_0802A238(struct SpriteHandler *handler, s16 spriteID, s8 *arg2) {
    sprite_set_visible(handler, spriteID, 0);
    *arg2 = 0;
}
