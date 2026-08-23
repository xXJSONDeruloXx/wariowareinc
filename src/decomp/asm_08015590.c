#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern void scene_set_current_thread(u32);
extern void func_080065C0(u32);

typedef struct Func08015590Scene {
    u8 paddingDE[0xDE];
    u8 flags;
    u8 paddingDF[0xDD];
    u32 field1BC;
    void (*callback)(void);
} Func08015590Scene;

void func_08015590(void) {
    Func08015590Scene **base;
    Func08015590Scene *scene;
    u8 *bytePtr;
    u32 value;
    u32 mask;

    scene_set_current_thread(0);
    base = (Func08015590Scene **)&gCurrentSceneData;
    scene = *base;
    func_080065C0(scene->field1BC);
    bytePtr = (u8 *)*base;
    bytePtr += 0xDE;
    value = *bytePtr;
    mask = 0x7F;
    mask &= value;
    *bytePtr = mask;
    scene = *base;
    scene->callback();
}
#endif
