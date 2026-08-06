#include "global.h"
#include "src/scenes/gameplay.h"

extern void scene_set_current_thread(u32);
extern void func_08004378(u32);
extern void func_0800418C(void);
extern void func_08016FD8(void);

void func_08024F68(void) {
    u8 *base = (u8 *)D_03006524;

    scene_set_current_thread(0);
    func_08004378(*(u32 *)(base + 0x50));
    func_0800418C();
    func_08016FD8();
}
