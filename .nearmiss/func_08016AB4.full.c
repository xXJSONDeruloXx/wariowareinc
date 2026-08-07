#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"

extern s16 func_0800A218(void);
extern void func_08007000(u32, s16, s16);
extern void func_08006FC0(s16, void *);
extern void func_0800806C(u32, u32, u32, u32, u32, u32);

void func_08016AB4(void) {
    u8 *scene;
    s16 sprite_id;

    sprite_id = func_0800A218();
    scene = (u8 *)gCurrentSceneData;
    *(s16 *)(scene + 0x3C) = sprite_id;
    *(s16 *)(scene + 0x48) = 0x100;
    func_08007000((u32)(scene + 0x3C), 0x100, 0);
    sprite_id = gCurrentSceneSpritePool[3];
    func_08006FC0(sprite_id, scene + 0x3C);
    func_0800806C(3, 0x8000, 0x7800, 0xF000, 0xA000, 0);
}
