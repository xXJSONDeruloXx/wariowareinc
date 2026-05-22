#if __INCLUDE_LEVEL__ > 0
#include "global.h"

void func_08014490(void) {
    register void **base asm("r4");
    register u32 zero asm("r5");
    u8 *data;
    register u8 *data2 asm("r0");

    scene_set_current_thread(0);
    base = &gCurrentSceneData;
    data = *base;
    zero = 0;
    *(u16 *)(data + 0x38) = 1;
    set_pause_beatscript_scene(0);
    data2 = *base;
    data2[8] = zero;
    func_0800C7A4(0);
}
#endif
