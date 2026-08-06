#include "global.h"

extern u16 get_current_mem_id(void);
extern s32 start_load_gfx_table_task(u16, const void *, u32);
extern void run_func_after_task(s32, void (*)(u32), s32);
extern void func_080166E4(u32);

void func_08016708(void) {
    s32 task = start_load_gfx_table_task(get_current_mem_id(),
                                         (const void *)0x083AB63C,
                                         0x3000);

    run_func_after_task(task, (void (*)(u32))(func_080166E4 + 1), 0);
}
