#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void func_0805AB2C(void) {
    sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + 0x94));
    sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + 0x98));
}