#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/task_pool.h"

extern void *start_new_task(u16, void *, void *, void *, u32);
extern u16 get_current_mem_id(void);
extern void scene_set_current_thread(u32);
extern void func_08012D3C(void);
extern u8 D_083A4B28[];

void func_08012D7C(void) {
    u32 stack_args[5];

    scene_set_current_thread(0);
    stack_args[1] = VRAMBase + 0x8000;
    stack_args[0] = VRAMBase + 0x8000;
    stack_args[2] = 0x4000;
    stack_args[3] = 0x1000;
    stack_args[4] = 4;
    run_func_after_task((s32)start_new_task((u16)get_current_mem_id(), (void *)D_083A4B28,
                                            &stack_args[0], NULL, 0),
                        (TaskFinalFunc)(func_08012D3C + 1), 0);
}
#endif
