#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);
extern void func_0801429C(u32, u32);
extern void func_08014374(void);

struct Func080143BCSceneV2 {
    u8 padding[0xDD];
    u8 fieldDD;
};

void func_080143BC(void) {
    struct Func080143BCSceneV2 **base;
    struct Func080143BCSceneV2 *scene;
    u8 *data;
    u32 value;
    u32 mask;

    scene_set_current_thread(0);
    base = (struct Func080143BCSceneV2 **)&gCurrentSceneData;
    data = (u8 *)*base;
    func_0801429C(data[0xFD], 1);
    func_08014374();
    scene = *base;
    value = scene->fieldDD;
    mask = 2;
    mask = -mask;
    mask &= value;
    scene->fieldDD = mask;
}
#endif
