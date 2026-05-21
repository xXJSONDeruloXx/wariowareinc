#include "global.h"
#include "src/lib_sprite.h"

void func_08067080(void) {
    if (*(u32*)((u8*)gCurrentSceneVariable + 0xE0) != 0) {
        sprite_id_delete(gSpriteHandler, *(u32*)((u8*)gCurrentSceneVariable + (0xC4 << 4)));
        sprite_id_delete(gSpriteHandler, *(u32*)((u8*)gCurrentSceneVariable + 0xC4C));
        sprite_id_delete(gSpriteHandler, *(u32*)((u8*)gCurrentSceneVariable + 0xC48));
        sprite_id_delete(gSpriteHandler, *(u32*)((u8*)gCurrentSceneVariable + 0xC44));
    }
}
