#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/task_pool.h"

extern void *start_new_texture_loader(u32, const void *);
extern void func_0800A3BC(void);

void func_0800A3D0(const void *texture) {
    s32 task;
    
    task = (s32)start_new_texture_loader(0, texture);
    run_func_after_task(task, (TaskFinalFunc)(func_0800A3BC + 1), 0);
    
    ((u8 *)gCurrentSceneData)[7] |= 2;
}
#endif
