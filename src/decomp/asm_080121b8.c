#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

struct Func080121B8Scene {
    u8 padding[0xDD];
    u8 fieldDD;
};

void func_080121B8(void) {
    struct Func080121B8Scene *scene;
    u32 value;
    u32 mask;

    scene = (struct Func080121B8Scene *)gCurrentSceneData;
    value = scene->fieldDD;
    mask = 3;
    mask = -mask;
    mask &= value;
    scene->fieldDD = mask;
}
#endif
