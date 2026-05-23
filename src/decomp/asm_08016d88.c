#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void func_08016DB8(void);
extern u32 func_08016DE0(void);
extern void func_080001D4(void);
extern u16 D_030035E0;
extern s16 gCurrentScene;

void func_08016D88(void) {
    register u32 r0 asm("r0");
    register u32 r4 asm("r4");

    r0 = (u32)&D_030035E0;
    r0 = *(s16 *)r0;
    if (r0 != 0) {
        func_08016DB8();
    }
    r4 = func_08016DE0();
    if (r4 == 1) {
        func_080001D4();
        r0 = (u32)&gCurrentScene;
        *(u16 *)r0 = r4;
    }
}
#endif
