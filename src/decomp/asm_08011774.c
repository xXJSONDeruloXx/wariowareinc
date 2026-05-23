#if __INCLUDE_LEVEL__ > 0
#include "global.h"

__attribute__((naked)) void func_08011774(void) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, lr}\n"
        "movs r4, #0\n"
        "1:\n"
        "ldr r0, _080117A0\n"
        "ldr r0, [r0]\n"
        "ldr r1, _080117A4\n"
        "ldr r2, [r1]\n"
        "lsls r1, r4, #1\n"
        "adds r1, r2\n"
        "movs r2, #2\n"
        "ldrsh r1, [r1, r2]\n"
        "movs r2, #1\n"
        "bl sprite_set_anim_cel\n"
        "adds r4, #1\n"
        "cmp r4, #2\n"
        "bls 1b\n"
        "movs r0, #0xa\n"
        "bl func_0800C7A4\n"
        "pop {r4}\n"
        "pop {r0}\n"
        "bx r0\n"
        ".balign 4, 0\n"
        "_080117A0:\n"
        ".word gSpriteHandler\n"
        "_080117A4:\n"
        ".word gCurrentSceneSpritePool\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
