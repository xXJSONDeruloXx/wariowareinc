#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void scene_set_current_thread(u32);
extern u16 get_current_mem_id(void);
extern s32 start_new_texture_loader(u16 memID, void **textureList);
extern void run_func_after_task(s32 poolID, void (*onFinish)(void), s32 onFinishArg);

extern void *D_083A9C14[];
extern void func_080109CC(void);

void func_080109EC(void) {
    u16 memID;
    s32 task;
    
    scene_set_current_thread(0);
    memID = get_current_mem_id();
    task = start_new_texture_loader(memID, D_083A9C14);
    run_func_after_task(task, func_080109CC, 0);
}
#endif
