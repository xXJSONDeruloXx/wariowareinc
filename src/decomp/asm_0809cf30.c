#include "global.h"
#include "types.h"
#include "src/scenes/title.h"

void func_0809CF30(void) {
    scene_set_current_thread(1);
    *(u32 *)((u8 *)gCurrentSceneVariable + (0xC6 << 1)) = 1;
}
