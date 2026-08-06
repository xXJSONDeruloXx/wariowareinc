#include "global.h"

extern void scene_set_current_thread(u32);
extern u16 get_current_mem_id(void);
extern s32 start_new_texture_loader(u16, void **);
extern void run_func_after_task(s32, void (*)(u32), s32);
extern void set_pause_beatscript_scene(u32);

void func_08016E9C(void) {
    scene_set_current_thread(0);
    run_func_after_task(start_new_texture_loader(get_current_mem_id(),
                                                 (void **)0x083AD840),
                        (void (*)(u32))(set_pause_beatscript_scene + 1),
                        0);
}
