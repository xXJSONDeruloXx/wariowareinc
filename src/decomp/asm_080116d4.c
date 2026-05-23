#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern void set_pause_beatscript_scene(u32);

void func_080116D4(void) {
    register void **base asm("r3");
    register u8 *bytePtr asm("r1");
    register u32 value asm("r2");
    register u32 mask asm("r0");
    register u8 *data asm("r0");
    register u32 offset asm("r1");
    register u32 val asm("r0");
    register u32 m2 asm("r1");

    base = &gCurrentSceneData;
    bytePtr = *base;
    bytePtr += 0xDF;
    value = *bytePtr;
    mask = 5;
    mask = -mask;
    mask &= value;
    *bytePtr = mask;

    data = *base;
    offset = 0x9E;
    offset <<= 1;
    data += offset;
    val = *(u32 *)data;
    m2 = 2;
    m2 = -m2;
    val &= m2;
    if (val != 0) {
        set_pause_beatscript_scene(1);
    }
}
#endif
