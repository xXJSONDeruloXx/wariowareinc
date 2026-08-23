#include "global.h"
#include "types.h"
#include "scenes.h"

struct Func0801D4B4Data {
    u16 value;
    u8 unk2;
    u8 flag3;
};

struct Func0801D4B4Scene {
    u8 padding[0xC];
    struct Func0801D4B4Data *data;
};

void func_0801D4B4(u32 value) {
    struct Func0801D4B4Data *data;

    data = ((struct Func0801D4B4Scene *)gCurrentSceneVariable)->data;
    value <<= 8;
    data->value = value;
    data->flag3 = 1;
}
