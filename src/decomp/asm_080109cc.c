#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/beatscript.h"

struct Func080109CCScene {
    u8 padding[0xDF];
    u8 fieldDF;
};

void func_080109CC(void) {
    struct Func080109CCScene *scene;
    u32 value;
    u32 mask;

    set_pause_beatscript_scene(0);
    scene = (struct Func080109CCScene *)gCurrentSceneData;
    value = scene->fieldDF;
    mask = 3;
    mask = -mask;
    mask &= value;
    scene->fieldDF = mask;
}
#endif
