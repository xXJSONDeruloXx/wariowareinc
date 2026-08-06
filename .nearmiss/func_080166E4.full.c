#include "global.h"

struct CompressedData;

extern u16 get_current_mem_id(void);
extern s32 start_new_texture_loader(u16, struct CompressedData **);
extern void run_func_after_task(s32, void (*)(u32), s32);
extern void set_pause_beatscript_scene(u32);

void func_080166E4(void) {
    run_func_after_task(
        start_new_texture_loader(get_current_mem_id(),
                                 (struct CompressedData **)0x083AB648),
        set_pause_beatscript_scene,
        0);
}
