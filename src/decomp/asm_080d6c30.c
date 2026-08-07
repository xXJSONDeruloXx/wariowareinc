#include "global.h"

struct Func080D6C30Scene {
    u8 pad0[0x40E];
    u16 value40E;
    u16 value410;
};

extern struct BeatscriptLocalData *gCurrentSceneVariable;

void func_080D6C30(void) {
    struct Func080D6C30Scene *scene;

    scene = (struct Func080D6C30Scene *)gCurrentSceneVariable;
    scene->value40E += 0x19;
    scene->value410 += 0x50;
}
