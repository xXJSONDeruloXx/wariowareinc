#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void func_080E4E0C(void) {
    sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + (0xC4 << 1)));
}
