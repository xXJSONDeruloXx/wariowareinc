#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void func_08001B28(s8 arg0);
void func_0800CDB0(u32);

void func_0803E96C(void) {
    func_08001B28(*(s8 *)((u8 *)gCurrentSceneVariable + 0xE4));
    sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + 0xE0));
    func_0800CDB0(1);
}
