#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern void scene_set_current_thread(u32);
extern void func_08014E88(s32);

__attribute__((naked))
void func_080152A0(void) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, lr}\n"
        "movs r0, #0\n"
        "bl scene_set_current_thread\n"
        "ldr r4, =gCurrentSceneData\n"
        "ldr r0, [r4]\n"
        "movs r1, #0xC2\n"
        "lsls r1, r1, #1\n"
        "adds r0, r0, r1\n"
        "movs r1, #0\n"
        "ldrsh r0, [r0, r1]\n"
        "bl func_08014E88\n"
        "ldr r1, [r4]\n"
        "adds r1, #0xDD\n"
        "ldrb r2, [r1]\n"
        "movs r0, #2\n"
        "rsbs r0, r0, #0\n"
        "ands r0, r2\n"
        "strb r0, [r1]\n"
        "pop {r4}\n"
        "pop {r0}\n"
        "bx r0\n"
        ".balign 4, 0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
