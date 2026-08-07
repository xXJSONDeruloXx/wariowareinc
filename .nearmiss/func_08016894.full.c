#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"

extern s16 func_0800A218(void);
extern void func_08007000(u32, s16, s16);
extern void func_08006FC0(s16, void *);

void func_08016894(void) {
    u8 *scene;
    s16 sprite_id;

    sprite_id = func_0800A218();
    scene = (u8 *)gCurrentSceneData;
    *(s16 *)(scene + 0x3C) = sprite_id;
    *(s16 *)(scene + 0x48) = 0x40;
    func_08007000((u32)(scene + 0x3C), 0x40, 0);
    sprite_id = gCurrentSceneSpritePool[3];
    func_08006FC0(sprite_id, scene + 0x3C);
    sprite_set_origin_x_y(gSpriteHandler, sprite_id, (s16 *)(scene + 0x4C), (s16 *)(scene + 0x4E));
    sprite_set_origin_x_y(gSpriteHandler, gCurrentSceneSpritePool[6], (s16 *)(scene + 0x4C), (s16 *)(scene + 0x4E));
    sprite_set_origin_x_y(gSpriteHandler, gCurrentSceneSpritePool[7], (s16 *)(scene + 0x4C), (s16 *)(scene + 0x4E));
    *(u8 *)(scene + 0x4A) = (u8)(-2 & *(u8 *)(scene + 0x4A));
    *(u8 *)(scene + 0x4A) = (u8)(-3 & *(u8 *)(scene + 0x4A));
    *(s16 *)(scene + 0x4C) = 0;
    *(s16 *)(scene + 0x4E) = 0;
}
