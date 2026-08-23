#include "global.h"
#include "types.h"

struct Func080C4A48Scene {
    u8 padding[8];
    u16 field8;
};

void func_080C4A48(u32 value) {
    struct Func080C4A48Scene *scene;
    s32 delta;
    u32 current;

    scene = (struct Func080C4A48Scene *)gCurrentSceneVariable;
    delta = (s16)value;
    current = scene->field8;
    delta += current;
    scene->field8 = delta;
}
