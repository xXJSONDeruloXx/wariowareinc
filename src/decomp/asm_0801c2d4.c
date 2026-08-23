#include "global.h"
#include "types.h"

typedef struct {
    u8 padding12C[0x12C];
    u8 field12C;
    u8 field12D;
} Func0801C2D4Scene;

void func_0801C2D4(u32 arg0) {
    Func0801C2D4Scene *scene;
    u32 value;

    scene = (Func0801C2D4Scene *)gCurrentSceneVariable;
    value = arg0;
    scene->field12C = value;
    scene = (Func0801C2D4Scene *)gCurrentSceneVariable;
    scene->field12D = value;
}
