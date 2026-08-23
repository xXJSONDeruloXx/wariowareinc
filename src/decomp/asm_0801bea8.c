#include "global.h"

struct Func0801BEA8Scene {
    u8 pad0[0x18];
    u8 field18;
};

void func_0801BEA8(void) {
    struct Func0801BEA8Scene *scene;
    u32 value;
    u32 mask;

    scene = (struct Func0801BEA8Scene *)gCurrentSceneVariable;
    value = scene->field18;
    mask = 0x3D;
    mask = -mask;
    mask &= value;
    mask |= 0x14;
    scene->field18 = mask;
}
