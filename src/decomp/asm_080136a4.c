#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/scenes/main_menu.h"

extern void scene_set_current_thread(u32);
extern u32 func_080135E8(u32);
extern void func_08015A88(void);

struct Func080136A4Scene {
    u8 padding[0xDD];
    u8 fieldDD;
};

void func_080136A4(void) {
    struct Func080136A4Scene *scene;
    u32 value;
    u32 mask;

    scene_set_current_thread(0);
    sprite_set_anim_cel(gSpriteHandler, gCurrentSceneSpritePool[6], 0);
    scene = (struct Func080136A4Scene *)gCurrentSceneData;
    value = scene->fieldDD;
    mask = 2;
    mask = -mask;
    mask &= value;
    scene->fieldDD = mask;
    func_080135E8(D_03006518.unk0);
    func_08015A88();
    D_03006518.unk1 = 2;
}
#endif
