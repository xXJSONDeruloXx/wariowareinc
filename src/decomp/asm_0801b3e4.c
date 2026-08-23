#include "global.h"

struct Func0801B3E4Scene {
    u8 pad0[0xF4];
    u8 fieldF4;
};

void func_0801B3E4(void) {
    struct Func0801B3E4Scene *scene;
    u32 value;
    u32 mask;

    scene = (struct Func0801B3E4Scene *)gCurrentSceneVariable;
    value = scene->fieldF4;
    mask = 2;
    mask = -mask;
    mask &= value;
    scene->fieldF4 = mask;
}
