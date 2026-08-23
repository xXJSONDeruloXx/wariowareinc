#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/code_08000f10.h"

extern void *gCurrentSceneData;

typedef struct Func080115DCScene {
    u8 paddingD4[0xD4];
    const void *source;
    void *destination;
    u8 flag;
} Func080115DCScene;

void func_080115DC(void) {
    Func080115DCScene **base;
    Func080115DCScene *scene;
    const void *source;
    void *destination;

    base = (Func080115DCScene **)&gCurrentSceneData;
    scene = *base;
    if (scene->flag != 0) {
        source = scene->source;
        destination = scene->destination;
        dma3_set(source, destination, 0xA0 << 3, 0x20, 0x80 << 1);
    }
}
#endif
