#include "global.h"
#include "src/lib_sprite.h"

extern u8 D_0300490E[];

void func_0801E44C(void) {
    register u8 *base asm("r1") = D_0300490E;
    register struct SpriteHandler *handler asm("r0") = gSpriteHandler;

    sprite_set_visible(handler, *(s16 *)(base + 0x12), 1);
}
