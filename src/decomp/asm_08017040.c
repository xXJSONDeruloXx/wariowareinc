#include "global.h"

extern void run_func_after_task(u32, void (*)(u32), u32);
extern void set_pause_beatscript_scene(u32);

void func_08017040(u32 a0) {
    run_func_after_task(a0, set_pause_beatscript_scene, 0);
}
