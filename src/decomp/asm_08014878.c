#if __INCLUDE_LEVEL__ > 0
#include "global.h"

/* gCurrentSceneData: from types.h */
/* scene_set_current_thread: from beatscript.h */
extern void func_08014810(u32);
extern void func_0800C77C(u32);

void func_08014878(void) {
    register u32 gptr asm("r0");
    register u8 *bytePtr asm("r1");
    register u32 value asm("r2");
    register u32 mask asm("r0");

    scene_set_current_thread(0);
    func_08014810(1);
    func_0800C77C(0x13);
    func_0800C77C(0x14);
    func_0800C77C(0x15);
    func_0800C77C(0x16);
    func_0800C77C(0x17);
    gptr = (u32)&gCurrentSceneData;
    bytePtr = *(u8 **)gptr;
    bytePtr += 0xDE;
    value = *bytePtr;
    mask = 0x11;
    mask = -mask;
    mask &= value;
    *bytePtr = mask;
}
#endif
