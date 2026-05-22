#if __INCLUDE_LEVEL__ > 0
#include "global.h"

__attribute__((naked))
void func_08014E88(s32 arg0) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, lr}\n"
        "adds r4, r0, #0\n"
        "bl func_08014E38\n"
        "ldr r0, =gSpriteHandler\n"
        "ldr r0, [r0]\n"
        "ldr r1, =gCurrentSceneData\n"
        "ldr r1, [r1]\n"
        "movs r2, #0xCA\n"
        "lsls r2, r2, #1\n"
        "adds r1, r1, r2\n"
        "ldr r1, [r1]\n"
        "lsls r4, r4, #1\n"
        "adds r4, r4, r1\n"
        "movs r2, #2\n"
        "ldrsh r1, [r4, r2]\n"
        "movs r2, #0xC\n"
        "bl sprite_set_base_palette\n"
        "pop {r4}\n"
        "pop {r0}\n"
        "bx r0\n"
        ".balign 4, 0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
