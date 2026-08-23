#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);
extern void func_080117FC(void);
extern void func_08015C38(void);
extern void func_08011730(u32);
extern struct Unk03006518 D_03006518;

struct Func080126C8Scene {
    u8 padding[0xDD];
    u8 fieldDD;
};

void func_080126C8(void) {
    struct Func080126C8Scene *scene;
    u32 value;
    u32 mask;

    scene_set_current_thread(0);
    D_03006518.unk1 = 0;
    func_080117FC();
    func_08015C38();
    func_08011730(1);
    scene = (struct Func080126C8Scene *)gCurrentSceneData;
    value = scene->fieldDD;
    mask = 2;
    mask = -mask;
    mask &= value;
    scene->fieldDD = mask;
}
#endif
