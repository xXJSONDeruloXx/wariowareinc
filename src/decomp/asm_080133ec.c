#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);
extern void func_08013AF4(void);
extern void func_08013A94(void);
extern void func_08013B94(void);
extern void func_08013C60(void);

struct Func080133ECScene {
    u8 padding[0xDD];
    u8 fieldDD;
};

void func_080133EC(void) {
    struct Func080133ECScene *scene;
    u32 value;
    u32 mask;

    scene_set_current_thread(0);
    func_08013AF4();
    func_08013A94();
    func_08013B94();
    D_03006518.unk1 = 3;
    func_08013C60();
    scene = (struct Func080133ECScene *)gCurrentSceneData;
    value = scene->fieldDD;
    mask = 2;
    mask = -mask;
    mask &= value;
    scene->fieldDD = mask;
}
#endif
