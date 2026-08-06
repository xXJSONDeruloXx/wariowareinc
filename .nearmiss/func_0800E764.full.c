#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"

s32 func_0800E764(void) {
    u8 *base = (u8 *)gCurrentSceneData;

    sprite_set_visible(gSpriteHandler, *(s16 *)(base + 0x2D0), 0);
}
