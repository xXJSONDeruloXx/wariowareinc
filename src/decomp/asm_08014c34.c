#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern void scene_set_current_thread(u32);
extern void func_0800C77C(u32);

void func_08014C34(void) {
    register void **base asm("r3");
    register u8 *bytePtr asm("r1");
    register u32 value asm("r2");
    register u32 mask asm("r0");
    register void *data asm("r0");
    register u32 offset asm("r1");
    register u32 funcPtr asm("r0");

    scene_set_current_thread(0);
    func_0800C77C(0x18);
    base = &gCurrentSceneData;
    bytePtr = *base;
    bytePtr += 0xDE;
    value = *bytePtr;
    mask = 0x21;
    mask = -mask;
    mask &= value;
    *bytePtr = mask;
    data = *base;
    offset = 0xBA;
    offset <<= 1;
    data += offset;
    funcPtr = *(u32 *)data;
    if (funcPtr != 0) {
        ((void (*)(void))funcPtr)();
    }
}
#endif
