#include "src/lib_sprite.h"

void func_08019644(u16 arg0) {
    sprite_set_visible(gSpriteHandler,
                       *(s16 *)((u8 *)gCurrentSceneVariable + 0x68), arg0);
}
