#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void func_0807E350(void) {
    sprite_set_enable_updates(gSpriteHandler, *(s16 *)((u8 *)gCurrentSceneVariable + 0x16), 1);
}
