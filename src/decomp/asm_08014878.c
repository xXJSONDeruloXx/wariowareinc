#if __INCLUDE_LEVEL__ > 0
#include "global.h"

/* gCurrentSceneData: from types.h */
/* scene_set_current_thread: from beatscript.h */
extern void func_08014810(u32);
extern void func_0800C77C(u32);

struct Func08014878Scene {
    u8 padding[0xDE];
    u8 fieldDE;
};

void func_08014878(void) {
    struct Func08014878Scene *scene;
    u32 value;
    u32 mask;

    scene_set_current_thread(0);
    func_08014810(1);
    func_0800C77C(0x13);
    func_0800C77C(0x14);
    func_0800C77C(0x15);
    func_0800C77C(0x16);
    func_0800C77C(0x17);
    scene = (struct Func08014878Scene *)gCurrentSceneData;
    value = scene->fieldDE;
    mask = 0x11;
    mask = -mask;
    mask &= value;
    scene->fieldDE = mask;
}
#endif
