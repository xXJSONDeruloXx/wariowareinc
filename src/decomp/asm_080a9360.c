#include "global.h"
#include "src/scenes/title.h"
#include "types.h"

void func_080A9360(void) {
    scene_set_current_thread(1);
    *(u8 *)gCurrentSceneVariable = 0;
}
