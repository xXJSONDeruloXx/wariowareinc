#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);

struct Func080148BCScene {
    u8 padding[0xDE];
    u8 fieldDE;
};

void func_080148BC(void) {
    struct Func080148BCScene **base;
    struct Func080148BCScene *scene;
    u8 *data;
    u32 value;
    u32 mask;
    u32 offset;
    void (*callback)(void);

    scene_set_current_thread(0);
    base = (struct Func080148BCScene **)&gCurrentSceneData;
    scene = *base;
    value = scene->fieldDE;
    mask = 0x11;
    mask = -mask;
    mask &= value;
    scene->fieldDE = mask;
    data = (u8 *)*base;
    offset = 0xA2;
    offset <<= 1;
    data += offset;
    callback = *(void (**)(void))data;
    callback();
}
#endif
