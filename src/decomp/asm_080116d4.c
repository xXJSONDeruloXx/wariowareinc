#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern void set_pause_beatscript_scene(u32);

void func_080116D4(void) {
    void **base;
    u8 *bytePtr;
    u32 value;
    u32 mask;
    u8 *data;
    u32 offset;
    u32 val;
    u32 m2;

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
