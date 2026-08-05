#include "global.h"
#include "src/lib_sprite.h"

void func_080D1034(void *arg0) {
    u8 *base = (u8 *)arg0;

    *(u8 *)(base + 0x1A) = 0;
    *(u8 *)(base + 0x17) = 0;
    sprite_set_visible(gSpriteHandler, *(s16 *)base, 0);
}
