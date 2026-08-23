#include "global.h"
#include "types.h"

typedef struct {
    u8 padding[0x1A0];
    u16 field1A0;
    u8 field1A2;
} Func080C6898Scene;

void func_080C6898(void) {
    Func080C6898Scene *scene;

    scene = (Func080C6898Scene *)gCurrentSceneVariable;
    scene->field1A0 = 0;
    scene->field1A2 = 0;
}
