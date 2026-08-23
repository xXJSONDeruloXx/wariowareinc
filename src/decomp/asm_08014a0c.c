#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);
extern void func_08014810(u32);

struct Func08014A0CScene {
    u8 padding[0xDD];
    u8 fieldDD;
};

void func_08014A0C(void) {
    struct Func08014A0CScene *scene;
    u32 value;
    u32 mask;

    scene_set_current_thread(0);
    func_08014810(1);
    scene = (struct Func08014A0CScene *)gCurrentSceneData;
    value = scene->fieldDD;
    mask = 2;
    mask = -mask;
    mask &= value;
    scene->fieldDD = mask;
}
#endif
