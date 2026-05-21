#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void func_0804E6C4(void) {
    sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + (0xD6 << 1)));
}
