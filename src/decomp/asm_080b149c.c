#include "global.h"
#include "types.h"
#include "src/scenes/title.h"

void func_080B27D8(void);

void func_080B149C(void) {
    scene_set_current_thread(1);
    ((u8 *)gCurrentSceneVariable)[0x25] = 0;
    func_080B27D8();
}
