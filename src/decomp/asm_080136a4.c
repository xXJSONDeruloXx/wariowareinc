#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "types.h"
#include "src/lib_sprite.h"
#include "src/scenes/main_menu.h"

extern void scene_set_current_thread(u32);
extern u32 func_080135E8(u32);
extern void func_08015A88(void);

typedef void (*Func080136A4SetAnimCel)(struct SpriteHandler *, s32, s32);

void func_080136A4(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");

    scene_set_current_thread(0);
    r0 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r0;
    r1 = (u32)&gCurrentSceneSpritePool;
    r1 = *(u32 *)r1;
    r2 = 0xC;
    r1 = *(s16 *)(r1 + r2);
    r2 = 0;
    ((Func080136A4SetAnimCel)sprite_set_anim_cel)((struct SpriteHandler *)r0, r1, r2);
    r0 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r0;
    r1 += 0xDD;
    r2 = *(u8 *)r1;
    r0 = 2;
    r0 = -r0;
    r0 &= r2;
    *(u8 *)r1 = r0;
    r4 = (u32)&D_03006518;
    r0 = *(u8 *)r4;
    func_080135E8(r0);
    func_08015A88();
    r0 = 2;
    *(u8 *)(r4 + 1) = r0;
}
#endif
