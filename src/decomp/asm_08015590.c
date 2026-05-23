#if __INCLUDE_LEVEL__ > 0
#include "global.h"

/* gCurrentSceneData: from types.h */
/* scene_set_current_thread: from beatscript.h */
extern void func_080065C0(u32);
typedef void (*CallbackFn15590)(void);

void func_08015590(void) {
    register void **base asm("r4");
    register u8 *data asm("r0");
    register u32 offset asm("r1");
    register u8 *bytePtr asm("r1");
    register u32 value asm("r2");
    register u32 mask asm("r0");

    scene_set_current_thread(0);
    base = &gCurrentSceneData;
    data = *(u8 **)base;
    offset = 0xDE;
    offset <<= 1;
    data += offset;
    func_080065C0(*(u32 *)data);
    bytePtr = *(u8 **)base;
    bytePtr += 0xDE;
    value = *bytePtr;
    mask = 0x7F;
    mask &= value;
    *bytePtr = mask;
    data = *(u8 **)base;
    offset = 0xE0;
    offset <<= 1;
    data += offset;
    ((CallbackFn15590)*(u32 *)data)();
}
#endif
