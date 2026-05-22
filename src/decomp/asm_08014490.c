#if __INCLUDE_LEVEL__ > 0
#include "global.h"

__attribute__((naked))
void func_08014490(void) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, r5, lr}\n"
        "movs r0, #0\n"
        "bl scene_set_current_thread\n"
        "ldr r4, =gCurrentSceneData\n"
        "ldr r1, [r4]\n"
        "movs r5, #0\n"
        "movs r0, #1\n"
        "strh r0, [r1, #0x38]\n"
        "movs r0, #0\n"
        "bl set_pause_beatscript_scene\n"
        "ldr r0, [r4]\n"
        "strb r5, [r0, #8]\n"
        "movs r0, #0\n"
        "bl func_0800C7A4\n"
        "pop {r4, r5}\n"
        "pop {r0}\n"
        "bx r0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
