#include "global.h"
#include "src/lib_sprite.h"

void func_080B83B0(void) {
    sprite_set_visible(gSpriteHandler, (s16)*(u32 *)((u8 *)gCurrentSceneVariable + 0x5C), 0);
}
