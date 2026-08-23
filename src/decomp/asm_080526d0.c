#include "global.h"

struct Func080526D0Scene {
    u8 padding[0x6C];
    s32 value6C;
    s32 value70;
};

void func_080526D0(void) {
    struct Func080526D0Scene *scene;
    s32 value;

    scene = (struct Func080526D0Scene *)gCurrentSceneVariable;
    value = scene->value6C;
    if ((value >> 8) <= 0xB3) {
        scene->value6C = value + scene->value70;
    }
}
