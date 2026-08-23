#include "global.h"

typedef struct {
    u8 padding4[4];
    u32 value4;
    u8 padding8[4];
    s16 deltaC;
} Func0806F0A0Scene;

void func_0806F0A0(void) {
    Func0806F0A0Scene *scene;
    s32 delta;
    u32 value;

    scene = (Func0806F0A0Scene *)gCurrentSceneVariable;
    delta = scene->deltaC;
    value = scene->value4;
    value += delta;
    scene->value4 = value;
    if ((s32)value > (s32)0xD000) {
        scene->value4 = 0xD000;
    }
}
