#include "global.h"

struct Func0801AF18Scene {
    u8 pad0[0x18];
    u8 field18;
};

void func_0801AF18(void) {
    struct Func0801AF18Scene *scene;
    u32 value;
    u32 mask;

    scene = (struct Func0801AF18Scene *)gCurrentSceneVariable;
    value = scene->field18;
    mask = 0x3D;
    mask = -mask;
    mask &= value;
    mask |= 8;
    scene->field18 = mask;
}
