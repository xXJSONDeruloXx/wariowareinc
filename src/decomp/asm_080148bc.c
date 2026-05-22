#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern void scene_set_current_thread(u32);

typedef void (*MainMenuCallback)(void);

void func_080148BC(void) {
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
    mask = 0x11;
    mask = -mask;
    mask &= value;
    *bytePtr = mask;

    data = *base;
    offset = 0xA2;
    offset <<= 1;
    data += offset;
    ((MainMenuCallback)*(u32 *)data)();
}
#endif
