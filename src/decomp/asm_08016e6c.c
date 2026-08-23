#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void func_08016CBC(u32);
extern u32 func_08016D00(void);
extern s16 D_030035E0;
extern s16 gCurrentScene;
extern u32 D_083AD90C;

void func_08016E6C(void) {
    if (D_030035E0 != 0) {
        func_08016CBC((u32)&D_083AD90C);
    }
    if (func_08016D00() != 0) {
        gCurrentScene = 5;
    }
}
#endif
