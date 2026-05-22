#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern void scene_set_current_thread(u32);

__attribute__((naked))
void func_08014C6C(void) {
    asm volatile(
        ".syntax unified\n"
        "push {lr}\n"
        "movs r0, #0\n"
        "bl scene_set_current_thread\n"
        "ldr r3, =gCurrentSceneData\n"
        "ldr r1, [r3]\n"
        "adds r1, #0xDE\n"
        "ldrb r2, [r1]\n"
        "movs r0, #0x21\n"
        "rsbs r0, r0, #0\n"
        "ands r0, r2\n"
        "strb r0, [r1]\n"
        "ldr r0, [r3]\n"
        "movs r1, #0xB8\n"
        "lsls r1, r1, #1\n"
        "adds r0, r0, r1\n"
        "ldr r0, [r0]\n"
        "bl _call_via_r0\n"
        "pop {r0}\n"
        "bx r0\n"
        ".balign 4, 0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
