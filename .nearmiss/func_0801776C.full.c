#include "global.h"
#include "src/lib_sprite.h"
#include "scenes.h"

extern s32 func_08008058(s32, s32, s32);
extern struct Animation *D_083ADD28[];

void func_0801776C(s32 arg0) {
    struct Animation **animations;
    s32 index;

    animations = D_083ADD28;
    index = func_08008058(((u8 *)gCurrentSceneData)[0x175] + arg0, 0, 4);
    sprite_set_anim(gSpriteHandler, gCurrentSceneSpritePool[3], animations[index], 0, 1, 0, 0);
}
