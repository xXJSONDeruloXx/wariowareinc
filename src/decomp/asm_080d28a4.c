#include "global.h"
#include "types.h"

typedef struct {
    u8 padding[0x3E8];
    u16 field3E8;
    u8 padding3EA[2];
    u8 field3EC;
} Func080D28A4Scene;

void func_080D28A4(void) {
    Func080D28A4Scene *scene;

    scene = (Func080D28A4Scene *)gCurrentSceneVariable;
    scene->field3E8 = 0;
    scene->field3EC = 0;
}
