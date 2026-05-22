#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern u32 D_083AA3C4[];
extern u32 save_is_stage_unlocked(u32);
extern u32 func_0800C874(u32);
extern u32 func_080020FC(u32);

__attribute__((naked))
void func_08012C80(u32 arg0) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, lr}\n"
        "adds r4, r0, #0\n"
        "bl save_is_stage_unlocked\n"
        "cmp r0, #0\n"
        "beq 1f\n"
        "ldr r1, =D_083AA3C4\n"
        "lsls r0, r4, #2\n"
        "adds r0, r0, r1\n"
        "ldr r0, [r0]\n"
        "bl func_0800C874\n"
        "bl func_080020FC\n"
        "ldr r1, =gCurrentSceneData\n"
        "ldr r1, [r1]\n"
        "adds r1, #0x84\n"
        "str r0, [r1]\n"
        "1:\n"
        "pop {r4}\n"
        "pop {r0}\n"
        "bx r0\n"
        ".balign 4, 0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
