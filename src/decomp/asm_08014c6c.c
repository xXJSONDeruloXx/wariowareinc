#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern void scene_set_current_thread(u32);

typedef void (*MainMenuCallback14C6C)(void);

void func_08014C6C(void) {
    register void **base asm("r3");
    register u8 *bytePtr asm("r1");
    register u32 value asm("r2");
    register u32 mask asm("r0");
    register u8 *data asm("r0");
    register u32 offset asm("r1");

    scene_set_current_thread(0);
    base = &gCurrentSceneData;
    bytePtr = *base;
    bytePtr += 0xDE;
    value = *bytePtr;
    mask = 0x21;
    mask = -mask;
    mask &= value;
    *bytePtr = mask;

    data = *base;
    offset = 0xB8;
    offset <<= 1;
    data += offset;
    ((MainMenuCallback14C6C)*(u32 *)data)();
}
#endif
