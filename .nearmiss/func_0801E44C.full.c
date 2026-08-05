#include "global.h"
#include "src/lib_sprite.h"

extern u8 D_0300490E[];

void func_0801E44C(void) {
    sprite_set_visible(gSpriteHandler, *(s16 *)(D_0300490E + 0x12), 1);
}
