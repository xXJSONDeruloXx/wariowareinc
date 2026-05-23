#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void func_08016CBC(u32);
extern u32 func_08016D00(void);
extern u16 D_030035E0;
extern s16 gCurrentScene;
extern u32 D_083AB754;

void func_080166AC(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");

    r0 = (u32)&D_030035E0;
    r1 = 0;
    r0 = *(s16 *)r0;
    if (r0 == 0) goto skip;
    r0 = (u32)&D_083AB754;
    func_08016CBC(r0);
skip:
    r0 = func_08016D00();
    if (r0 == 0) goto done;
    r1 = (u32)&gCurrentScene;
    r0 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r0;
    r0 = *(u16 *)(r0 + 0x38);
    *(u16 *)r1 = r0;
done:;
}
#endif
