#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);

struct Func08014C6CScene {
    u8 padding[0xDE];
    u8 fieldDE;
};

void func_08014C6C(void) {
    struct Func08014C6CScene **base;
    struct Func08014C6CScene *scene;
    u8 *data;
    u32 value;
    u32 mask;
    u32 offset;
    void (*callback)(void);

    scene_set_current_thread(0);
    base = (struct Func08014C6CScene **)&gCurrentSceneData;
    scene = *base;
    value = scene->fieldDE;
    mask = 0x21;
    mask = -mask;
    mask &= value;
    scene->fieldDE = mask;
    data = (u8 *)*base;
    offset = 0xB8;
    offset <<= 1;
    data += offset;
    callback = *(void (**)(void))data;
    callback();
}
#endif
