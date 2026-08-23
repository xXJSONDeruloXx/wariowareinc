#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void func_08016DB8(void);
extern u32 func_08016DE0(void);
extern void func_080001D4(void);
extern s16 D_030035E0;
extern s16 gCurrentScene;

void func_08016D88(void) {
    u32 result;

    if (D_030035E0 != 0) {
        func_08016DB8();
    }
    result = func_08016DE0();
    if (result == 1) {
        func_080001D4();
        gCurrentScene = result;
    }
}
#endif
