#include "global.h"
#include "types.h"
#include "graphics.h"

struct Func080195E4SceneVariable {
    u8 padding[0x66];
    u16 value66;
};

struct Func080195E4Graphics {
    u8 padding[0x4C];
    u16 value4C;
};

void func_080195E4(void) {
    struct Func080195E4SceneVariable *scene;
    struct Func080195E4Graphics *graphics;

    scene = (struct Func080195E4SceneVariable *)gCurrentSceneVariable;
    scene->value66 = 0;
    graphics = (struct Func080195E4Graphics *)&gGraphicsBuffer;
    graphics->value4C = 0;
}
