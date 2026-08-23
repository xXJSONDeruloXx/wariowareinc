#include "global.h"
#include "src/lib_sprite.h"

extern u8 D_0300490E[];

void func_0801E44C(void) {
    u8 *base = D_0300490E;
    struct SpriteHandler *handler = gSpriteHandler;

    sprite_set_visible(handler, *(s16 *)(base + 0x12), 1);
}
