#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "types.h"
#include "src/lib_sprite.h"
#include "src/scenes/main_menu.h"

extern u32 func_08005920(void *);
extern void func_08005834(void *);
extern void sprite_set_x_y(void *, s32, s32, s32);

void func_08011584(void) {
    u32 value = *(u32 *)((u8 *)gCurrentSceneData + 0x1AC);

    if (func_08005920((void *)value) != 1)
        return;
    sprite_set_x_y((void *)gSpriteHandler, gCurrentSceneSpritePool[0],
                   *(s16 *)((u8 *)gCurrentSceneData + 0x1B0),
                   *(s16 *)((u8 *)gCurrentSceneData + 0x1B2));
    func_08005834((void *)value);
}
#endif
