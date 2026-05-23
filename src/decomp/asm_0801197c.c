#if __INCLUDE_LEVEL__ > 0
#include "global.h"

/* D_03006518, gCurrentSceneData: from main_menu.h/types.h */
/* scene_set_current_thread: from beatscript.h */
/* func_08011824: defined in another included_stub in this TU */
extern u32 func_080135E8(u32);
extern void func_08015A88(void);

void func_0801197C(void) {
    register u32 *base asm("r4");
    register u8 *bytePtr asm("r1");
    register u32 value asm("r2");
    register u32 mask asm("r0");
    register void **gptr asm("r0");

    scene_set_current_thread(0);
    base = (u32 *)&D_03006518;
    mask = 2;
    ((u8 *)base)[1] = mask;
    func_08011824();
    mask = *(u8 *)base;
    func_080135E8(mask);
    func_08015A88();
    gptr = &gCurrentSceneData;
    bytePtr = *gptr;
    bytePtr += 0xDD;
    value = *bytePtr;
    mask = 2;
    mask = -mask;
    mask &= value;
    *bytePtr = mask;
}
#endif
