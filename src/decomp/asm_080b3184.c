#include "global.h"
#include "types.h"
#include "src/scenes/title.h"

void func_080B3184(void) {
    u8 *p;
    scene_set_current_thread(1);
    p = (u8 *)gCurrentSceneVariable;
    *(u32 *)(p + 0x3C) = 0;
    *(u32 *)(p + 0x9C) = 0;
}
