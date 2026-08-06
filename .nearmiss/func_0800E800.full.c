#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"

extern void sprite_set_x_y(void *, s32, s32, s32);

void func_0800E800(u32 arg0, u32 arg1) {
    u8 *base = (u8 *)gCurrentSceneData;

    sprite_set_x_y(gSpriteHandler, *(s16 *)(base + 0x2D0), (s16)arg0, (s16)arg1);
}
