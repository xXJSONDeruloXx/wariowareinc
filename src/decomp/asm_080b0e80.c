#include "global.h"
#include "types.h"
#include "graphics.h"
#include "src/lib_sprite.h"

void func_0800CDB0(u32);

void func_080B0E80(void) {
    sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + 0x48));
    sprite_id_delete(gSpriteHandler, *(u32 *)((u8 *)gCurrentSceneVariable + (0xD0 << 1)));
    gGraphicsBuffer.unk4C = 0;
    *(u16 *)((u8 *)&gGraphicsBuffer + 0x4E) = 0;
    func_0800CDB0(1);
}
