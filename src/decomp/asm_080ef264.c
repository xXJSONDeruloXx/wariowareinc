#if __INCLUDE_LEVEL__ > 0
#include "global.h"

__attribute__((naked)) void sprite_set_x(void) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, r5, r6, lr}\n"
        "adds r5, r0, #0\n"
        "lsls r2, r2, #16\n"
        "lsrs r6, r2, #16\n"
        "ldr r2, _080EF294\n"
        "movs r0, #7\n"
        "strb r0, [r2]\n"
        "lsls r1, r1, #16\n"
        "asrs r4, r1, #16\n"
        "adds r0, r5, #0\n"
        "adds r1, r4, #0\n"
        "bl sprite_is_invalid\n"
        "cmp r0, #0\n"
        "bne 1f\n"
        "ldr r1, [r5, #8]\n"
        "lsls r0, r4, #3\n"
        "subs r0, r4\n"
        "lsls r0, r0, #3\n"
        "adds r0, r1\n"
        "strh r6, [r0, #2]\n"
        "1:\n"
        "pop {r4, r5, r6}\n"
        "pop {r0}\n"
        "bx r0\n"
        ".balign 4, 0\n"
        "_080EF294:\n"
        ".word D_03000E70\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
