#if __INCLUDE_LEVEL__ > 0
#include "global.h"

__attribute__((naked))
s16 sprite_get_anim_duration(struct Animation *anim) {
    asm volatile(
        ".syntax unified\n"
        "push {lr}\n"
        "adds r1, r0, #0\n"
        "movs r2, #0\n"
        "b 1f\n"
        "2:\n"
        "ldrb r0, [r1, #4]\n"
        "adds r0, r2, r0\n"
        "lsls r0, r0, #0x10\n"
        "lsrs r2, r0, #0x10\n"
        "adds r1, #8\n"
        "1:\n"
        "ldr r0, [r1]\n"
        "cmp r0, #0\n"
        "bne 2b\n"
        "adds r0, r2, #0\n"
        "pop {r1}\n"
        "bx r1\n"
        ".short 0x0000\n"
        ".syntax divided\n"
    );
}
#endif
