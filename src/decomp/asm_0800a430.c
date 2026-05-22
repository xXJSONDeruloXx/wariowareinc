#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 D_083A4BF0[];

__attribute__((naked))
s32 func_0800A430(s32 arg0) {
    asm volatile(
        ".syntax unified\n"
        "push {lr}\n"
        "adds r2, r0, #0\n"
        "ldr r1, =D_083A4BF0\n"
        "b 1f\n"
        ".balign 4, 0\n"
        ".ltorg\n"
        "2:\n"
        "ldr r0, [r1]\n"
        "cmp r0, r2\n"
        "bne 3f\n"
        "ldr r0, [r1, #4]\n"
        "b 4f\n"
        "3:\n"
        "adds r1, #8\n"
        "1:\n"
        "ldr r0, [r1]\n"
        "cmp r0, #0\n"
        "bne 2b\n"
        "movs r0, #0x8C\n"
        "4:\n"
        "pop {r1}\n"
        "bx r1\n"
        ".syntax divided\n"
    );
}
#endif
