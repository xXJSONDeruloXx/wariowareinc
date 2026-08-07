#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"
#include "src/scenes/title.h"

extern u8 D_083AD81C[];
extern u32 func_0800A3FC(u32, u32);
extern void func_08016EF8(void);

void func_08016F14(void) {
    u32 sprite_id;

    sprite_id = func_0800A3FC(0x300, 4);
    *(u32 *)gCurrentSceneData = sprite_id;
    func_08005538(gSpriteHandler, sprite_id, D_083AD81C, gCurrentSceneSpritePool);
    func_08016EF8();
    *(u8 *)((u8 *)gCurrentSceneData + 4) = 0;
}
