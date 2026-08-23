#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);
extern void func_0800C77C(u32);

struct Func08014C34Scene {
    u8 padding[0xDE];
    u8 fieldDE;
};

void func_08014C34(void) {
    struct Func08014C34Scene **base;
    struct Func08014C34Scene *scene;
    u8 *data;
    u32 value;
    u32 mask;
    u32 offset;
    void (*callback)(void);

    scene_set_current_thread(0);
    func_0800C77C(0x18);
    base = (struct Func08014C34Scene **)&gCurrentSceneData;
    scene = *base;
    value = scene->fieldDE;
    mask = 0x21;
    mask = -mask;
    mask &= value;
    scene->fieldDE = mask;
    data = (u8 *)*base;
    offset = 0xBA;
    offset <<= 1;
    data += offset;
    callback = *(void (**)(void))data;
    if (callback != 0) {
        callback();
    }
}
#endif
