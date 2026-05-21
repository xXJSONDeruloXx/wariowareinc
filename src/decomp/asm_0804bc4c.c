#include "global.h"
#include "graphics.h"
#include "src/lib_sprite.h"

void func_0804BC4C(void) {
    gGraphicsBuffer.DISPCNT &= 0xDFFF;
    *(u16*)((u8*)&gGraphicsBuffer + 0x46) = 0;
    *(u16*)((u8*)&gGraphicsBuffer + 0x44) = 0;
    *(u16*)((u8*)&gGraphicsBuffer + 0x3C) = 0;
    *(u16*)((u8*)&gGraphicsBuffer + 0x40) = 0;
    sprite_id_delete(gSpriteHandler, *(u32*)((u8*)gCurrentSceneVariable + 0xE4));
    func_08001B28(*(s8*)((u8*)gCurrentSceneVariable + 0xCA));
}
