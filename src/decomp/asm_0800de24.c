#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern u16 D_030035E0;
extern u8 D_083A9AF0;
extern s16 gCurrentScene;
extern u8 D_03003848;
extern u32 D_03003628;
extern u8 D_03003634;
extern u8 D_083A8588;
extern void func_08016CBC(u32);
extern u32 func_08016D00(void);
extern u32 func_080007C0(u32);

void func_0800DE24(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");

    r0 = (u32)&D_030035E0;
    r1 = 0;
    r0 = *(s16 *)(r0 + r1);
    if (r0 == 0) goto check_func;
    r0 = (u32)&D_083A9AF0;
    func_08016CBC(r0);

check_func:
    r0 = func_08016D00();
    if (r0 == 0) goto done;

    r4 = (u32)&gCurrentScene;
    r0 = 4;
    *(u16 *)r4 = r0;
    r0 = 2;
    r0 = func_080007C0(r0);
    r2 = r0;
    if (r2 != 0) goto done;

    r0 = 2;
    *(u16 *)r4 = r0;
    r0 = (u32)&D_03003848;
    *(u8 *)r0 = r2;
    r1 = (u32)&D_03003628;
    r0 = (u32)&D_083A8588;
    *(u32 *)r1 = r0;
    r0 = (u32)&D_03003634;
    *(u8 *)r0 = r2;

done:
    return;
}
#endif
