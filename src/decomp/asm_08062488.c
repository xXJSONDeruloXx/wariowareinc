#include "global.h"

struct Func08062488Scene {
    u8 padding[0xBD4];
    u8 valueBD4;
};

void func_08062488(void) {
    struct Func08062488Scene *scene;
    u32 value;
    u32 mask;

    scene = (struct Func08062488Scene *)gCurrentSceneVariable;
    value = scene->valueBD4;
    mask = 2;
    mask = -mask;
    mask &= value;
    scene->valueBD4 = mask;
}
