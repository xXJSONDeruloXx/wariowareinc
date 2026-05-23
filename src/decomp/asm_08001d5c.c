#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 D_03000010[];

__attribute__((naked)) void func_08001D5C(s32 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, r5, r6, lr}\n"
        "adds r4, r0, #0\n"
        "ldr r0, [sp, #0x10]\n"
        "lsls r1, r1, #0x10\n"
        "lsrs r1, r1, #0x10\n"
        "lsls r2, r2, #0x10\n"
        "lsrs r5, r2, #0x10\n"
        "lsls r3, r3, #0x10\n"
        "lsrs r3, r3, #0x10\n"
        "lsls r0, r0, #0x10\n"
        "lsrs r6, r0, #0x10\n"
        "cmp r4, #0\n"
        "blt 0f\n"
        "ldr r2, =D_03000010\n"
        "lsls r0, r4, #3\n"
        "adds r0, r0, r2\n"
        "strh r1, [r0]\n"
        "lsls r1, r4, #2\n"
        "adds r0, r1, #1\n"
        "lsls r0, r0, #1\n"
        "adds r0, r0, r2\n"
        "strh r5, [r0]\n"
        "adds r0, r1, #2\n"
        "lsls r0, r0, #1\n"
        "adds r0, r0, r2\n"
        "strh r3, [r0]\n"
        "adds r1, #3\n"
        "lsls r1, r1, #1\n"
        "adds r1, r1, r2\n"
        "strh r6, [r1]\n"
        "0:\n"
        "pop {r4, r5, r6}\n"
        "pop {r0}\n"
        "bx r0\n"
        ".balign 4, 0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
