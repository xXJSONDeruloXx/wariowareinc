#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern void scene_set_current_thread(u32);
extern void func_08014E88(s32);

void func_080152A0(void) {
    register void **base asm("r4");
    register u8 *data asm("r0");
    register u32 offset asm("r1");
    register s32 value asm("r0");
    register u8 *bytePtr asm("r1");
    register u32 byte asm("r2");
    register u32 mask asm("r0");

    scene_set_current_thread(0);
    base = &gCurrentSceneData;
    data = *base;
    offset = 0xC2;
    offset <<= 1;
    data += offset;
    offset = 0;
    value = *(s16 *)(data + offset);
    func_08014E88(value);

    bytePtr = *base;
    bytePtr += 0xDD;
    byte = *bytePtr;
    mask = 2;
    mask = -mask;
    mask &= byte;
    *bytePtr = mask;
}
#endif
