#include "global.h"
#include "types.h"
#include "src/scenes/title.h"

void func_08058A78(void);

void func_08058A5C(void) {
    scene_set_current_thread(1);
    func_08058A78();
    ((u8 *)gCurrentSceneVariable)[0x1D] = 0;
}
