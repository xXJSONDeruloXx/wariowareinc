#include "src/lib_sprite.h"

void func_08019644(u32 arg0) {
    u32 value;

    value = arg0;
    sprite_set_visible(gSpriteHandler,
                       *(s16 *)((u8 *)gCurrentSceneVariable + 0x68), (u16)value);
}
