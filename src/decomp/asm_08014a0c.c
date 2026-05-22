#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);
extern void func_08014810(u32);

void func_08014A0C(void) {
    u8 *ptr;
    u8 val;
    register u32 m asm("r0");
    scene_set_current_thread(0);
    func_08014810(1);
    ptr = (u8 *)gCurrentSceneData;
    ptr = ptr + 0xDD;
    val = ptr[0];
    m = 2;
    m = -m;
    m = val & m;
    ptr[0] = m;
}
#endif
