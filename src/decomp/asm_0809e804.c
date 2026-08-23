#include "scenes.h"

typedef struct {
    u8 paddingCA[0xCA];
    u16 fieldCA;
} Func0809E804Scene;

typedef struct {
    u8 padding16[0x16];
    u16 field16;
} Func0809E804Data;

void func_0809E804(s32 arg0) {
    Func0809E804Scene *scene;
    Func0809E804Data *data;
    u32 factor;
    u32 value;

    scene = (Func0809E804Scene *)gCurrentSceneVariable;
    data = (Func0809E804Data *)gCurrentSceneData;
    factor = data->field16;
    factor >>= 5;
    value = arg0;
    value *= factor;
    scene->fieldCA = value;
}
