#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void func_08016CBC(u32);
extern u32 func_08016D00(void);
extern s16 D_030035E0;
extern s16 gCurrentScene;
extern u32 D_083AB754;

struct Func080166ACSceneData {
    u8 padding[0x38];
    u16 value38;
};

void func_080166AC(void) {
    if (D_030035E0 != 0) {
        func_08016CBC((u32)&D_083AB754);
    }
    if (func_08016D00() != 0) {
        gCurrentScene = ((struct Func080166ACSceneData *)gCurrentSceneData)->value38;
    }
}
#endif
