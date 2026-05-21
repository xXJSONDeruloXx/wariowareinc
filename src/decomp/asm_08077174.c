#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void func_08077174(void) {
    sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + (0xE6 << 1)));
}