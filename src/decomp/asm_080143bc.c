#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern void scene_set_current_thread(u32);
extern void func_0801429C(u32, u32);
extern void func_08014374(void);

void func_080143BC(void) {
    register void **base asm("r4");
    register u8 *data asm("r0");
    register u8 *bytePtr asm("r1");
    register u32 value asm("r2");
    register u32 mask asm("r0");

    scene_set_current_thread(0);
    base = &gCurrentSceneData;
    data = *base;
    data += 0xFD;
    func_0801429C(*data, 1);
    func_08014374();
    bytePtr = *base;
    bytePtr += 0xDD;
    value = *bytePtr;
    mask = 2;
    mask = -mask;
    mask &= value;
    *bytePtr = mask;
}
#endif
