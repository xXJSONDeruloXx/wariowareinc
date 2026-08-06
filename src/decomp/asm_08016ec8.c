#include "global.h"

extern void scene_set_current_thread(u32);
extern u16 get_current_mem_id(void);
extern s32 start_load_gfx_table_task(u16, const void *, u32);
extern void run_func_after_task(s32, void (*)(u32), s32);
extern void func_08016E9C(u32);

void func_08016EC8(void) {
    scene_set_current_thread(0);
    run_func_after_task(start_load_gfx_table_task(get_current_mem_id(),
                                                  (const void *)0x083AD834,
                                                  0x3000),
                        (void (*)(u32))(func_08016E9C + 1),
                        0);
}
