#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

struct Func08013114Scene {
    u8 padding[0xDD];
    u8 fieldDD;
};

void func_08013114(void) {
    struct Func08013114Scene *scene;
    u32 value;
    u32 mask;

    scene = (struct Func08013114Scene *)gCurrentSceneData;
    value = scene->fieldDD;
    mask = 9;
    mask = -mask;
    mask &= value;
    scene->fieldDD = mask;
}
#endif
