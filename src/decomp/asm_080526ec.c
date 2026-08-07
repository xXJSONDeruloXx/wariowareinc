#include "global.h"
#include "scenes.h"

struct Func080526ECSceneData {
    u8 pad0[0x16];
    u16 value;
};

struct Func080526ECSceneVariable {
    u8 pad0[0x70];
    s32 value;
};

extern struct BeatscriptLocalData *gCurrentSceneVariable;

void func_080526EC(void) {
    struct Func080526ECSceneVariable *variable;
    struct Func080526ECSceneData *data;
    u16 value;

    variable = (struct Func080526ECSceneVariable *)gCurrentSceneVariable;
    data = (struct Func080526ECSceneData *)gCurrentSceneData;
    value = data->value;
    variable->value += (s32)(value * 0x50 * value) >> 0x10;
}
