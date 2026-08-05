#include "global.h"

extern void scene_set_current_thread(u32);
extern void func_0800C77C(u32);
extern u32 play_sound(void *);
extern u8 D_083FC594;

void func_08024494(void) {
    scene_set_current_thread(0);
    func_0800C77C(2);
    play_sound(&D_083FC594);
}
