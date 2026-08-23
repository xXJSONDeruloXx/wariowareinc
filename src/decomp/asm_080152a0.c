#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern void scene_set_current_thread(u32);
extern void func_08014E88(s32);

typedef struct {
    u8 paddingDD[0xDD];
    u8 fieldDD;
} Func080152A0Scene;

void func_080152A0(void) {
    void **base;
    u8 *data;
    u32 offset;
    s32 value;
    Func080152A0Scene *scene;
    u32 byte;
    u32 mask;

    scene_set_current_thread(0);
    base = &gCurrentSceneData;
    data = *base;
    offset = 0xC2;
    offset <<= 1;
    data += offset;
    offset = 0;
    value = *(s16 *)(data + offset);
    func_08014E88(value);
    scene = (Func080152A0Scene *)*base;
    byte = scene->fieldDD;
    mask = 2;
    mask = -mask;
    mask &= byte;
    scene->fieldDD = mask;
}
#endif
