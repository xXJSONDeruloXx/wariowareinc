#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void func_080C9050(void) {
    u32 id = *(u32 *)((u8 *)gCurrentSceneVariable + 0x574);
    sprite_id_delete(gSpriteHandler, id);
}