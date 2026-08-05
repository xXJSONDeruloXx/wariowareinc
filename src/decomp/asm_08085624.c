#include "global.h"
#include "src/lib_sprite.h"

void func_08085624(void) {
    sprite_set_visible(gSpriteHandler, *(s16 *)((u8 *)gCurrentSceneVariable + 0xC), 0);
}
