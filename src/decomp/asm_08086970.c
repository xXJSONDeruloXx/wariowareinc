#include "global.h"
#include "scenes.h"

struct Func08086970SceneData {
    u8 pad0[0x16];
    u16 value;
};

struct Func08086970SceneVariable {
    u8 pad0[0x20];
    s32 value;
};

extern struct BeatscriptLocalData *gCurrentSceneVariable;

void func_08086970(void) {
    struct Func08086970SceneVariable *variable;
    struct Func08086970SceneData *data;
    u16 value;

    variable = (struct Func08086970SceneVariable *)gCurrentSceneVariable;
    data = (struct Func08086970SceneData *)gCurrentSceneData;
    value = data->value;
    variable->value += (s32)(value * 0x48 * value) >> 0x10;
}
