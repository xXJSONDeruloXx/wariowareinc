#include "global.h"

typedef struct {
    u8 padding38[0x38];
    u32 field38;
} Func08089648Arg;

typedef struct {
    u8 padding3C[0x3C];
    u32 field3C;
} Func08089648Scene;

s32 func_08089648(void *arg0, u32 arg1) {
    Func08089648Arg *arg;
    Func08089648Scene *scene;
    u32 value;
    u32 difference;
    s32 result;

    result = 0;
    arg = (Func08089648Arg *)arg0;
    value = arg->field38;
    scene = (Func08089648Scene *)gCurrentSceneVariable;
    difference = scene->field3C - value;
    if (difference < arg1) {
        result = 1;
    }
    return result;
}
