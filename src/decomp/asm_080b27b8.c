#include "global.h"
#include "types.h"

typedef struct {
    u8 padding[0x1C8];
    u16 field1C8;
    u8 field1CA;
} Func080B27B8Scene;

void func_080B27B8(void) {
    Func080B27B8Scene *scene;

    scene = (Func080B27B8Scene *)gCurrentSceneVariable;
    scene->field1C8 = 0;
    scene->field1CA = 1;
}
