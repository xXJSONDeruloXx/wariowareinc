#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

struct Func0800A3BCScene {
    u8 padding[7];
    u8 field7;
};

void func_0800A3BC(void) {
    struct Func0800A3BCScene *scene;
    u32 value;
    u32 mask;

    scene = (struct Func0800A3BCScene *)gCurrentSceneData;
    value = scene->field7;
    mask = 3;
    mask = -mask;
    mask &= value;
    scene->field7 = mask;
}
#endif
