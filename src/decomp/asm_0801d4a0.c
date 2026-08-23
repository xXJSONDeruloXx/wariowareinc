#include "global.h"
#include "types.h"
#include "scenes.h"

struct Func0801D4A0Data {
    u16 value;
    u8 unk2;
    u8 flag3;
};

struct Func0801D4A0Scene {
    u8 pad0[0xC];
    struct Func0801D4A0Data *data;
};

void func_0801D4A0(u32 value) {
    struct Func0801D4A0Scene *scene = (struct Func0801D4A0Scene *)gCurrentSceneVariable;
    struct Func0801D4A0Data *data = scene->data;
    value <<= 8;
    data->value = value;
    data->flag3 = 0;
}
