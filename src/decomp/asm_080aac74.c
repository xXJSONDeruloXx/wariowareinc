#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void func_080AAC74(void) {
    sprite_id_delete(gSpriteHandler, ((u32 *)gCurrentSceneVariable)[7]);
}
