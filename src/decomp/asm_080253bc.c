#include "global.h"
#include "src/scenes/gameplay.h"

extern void scene_set_current_thread(u32);
extern void func_08004378(u32);
extern void func_08016FD8(void);

void func_080253BC(void) {
    scene_set_current_thread(0);
    func_08004378(*(u32 *)((u8 *)D_03006528 + 8));
    func_08016FD8();
}
