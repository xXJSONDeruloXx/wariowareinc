#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

void func_0800CDB0(u32);

void func_080C4754(void) {
    func_0800CDB0(1);
    sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + (0x94 << 1)));
}
