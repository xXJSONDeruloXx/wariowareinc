#include "global.h"
#include "graphics.h"
#include "src/beatscript.h"
#include "src/scenes/gameplay.h"
#include "src/task_pool.h"

extern void func_08016D3C(void);

s32 func_08016D00(void) {
    flush_graphics_buffer();
    trigger_pending_dma3();
    update_paused_beatscript_scene();
    task_pool_update_constant();
    task_pool_update_delayed();
    update_active_beatscript_scene();
    if (beatscript_scene_is_inactive() != 0) {
        func_08016D3C();
        return 1;
    }
    {
        func_08006F68();
        func_08006B00();
        func_080041B4();
        return 0;
    }
}
