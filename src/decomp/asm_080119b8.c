#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);
extern void func_08011824(void);
extern void func_080143A0(void);

struct Func080119B8Scene {
    u8 padding[0xDD];
    u8 fieldDD;
};

void func_080119B8(void) {
    struct Func080119B8Scene *scene;
    u32 value;
    u32 mask;

    scene_set_current_thread(0);
    D_03006518.unk1 = 4;
    func_08011824();
    scene = (struct Func080119B8Scene *)gCurrentSceneData;
    value = scene->fieldDD;
    mask = 2;
    mask = -mask;
    mask &= value;
    scene->fieldDD = mask;
    func_080143A0();
}
#endif
