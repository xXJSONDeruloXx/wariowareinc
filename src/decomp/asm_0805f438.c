#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void func_08001B28(s8 arg0);

void func_0805F438(void) {
    func_08001B28(*(s8 *)((u8 *)gCurrentSceneVariable + 0x46));
    sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + (0xAA << 2)));
}
