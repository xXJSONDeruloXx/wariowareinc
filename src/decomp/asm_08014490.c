#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);
extern void set_pause_beatscript_scene(u32);
extern void func_0800C7A4(s32);

struct Func08014490SceneV3 {
    u8 field8Padding[8];
    u8 field8;
    u8 padding[0x2F];
    u16 field38;
};

void func_08014490(void) {
    struct Func08014490SceneV3 **base;
    struct Func08014490SceneV3 *scene;
    u8 *data;
    u32 zero;

    scene_set_current_thread(0);
    base = (struct Func08014490SceneV3 **)&gCurrentSceneData;
    scene = *base;
    zero = 0;
    scene->field38 = 1;
    set_pause_beatscript_scene(0);
    data = (u8 *)*base;
    data[8] = zero;
    func_0800C7A4(0);
}
#endif
