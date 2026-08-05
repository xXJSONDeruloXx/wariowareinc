#include "global.h"
#include "src/lib_sprite.h"

void func_0808AB98(void *arg0) {
    u8 *base = (u8 *)arg0;

    sprite_set_enable_updates(gSpriteHandler, *(s16 *)base, 0);
    *(u8 *)(base + 0x18) = 0;
}
