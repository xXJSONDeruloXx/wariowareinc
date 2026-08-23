#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);

struct Func080144BCScene {
    u8 padding[0xDE];
    u8 fieldDE;
};

void func_080144BC(void) {
    struct Func080144BCScene *scene;
    u32 value;
    u32 mask;

    scene_set_current_thread(0);
    scene = (struct Func080144BCScene *)gCurrentSceneData;
    value = scene->fieldDE;
    mask = 9;
    mask = -mask;
    mask &= value;
    scene->fieldDE = mask;
}
#endif
