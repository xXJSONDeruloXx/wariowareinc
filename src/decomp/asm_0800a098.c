#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;

__attribute__((naked))
u32 func_0800A098(void) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, lr}\n"
        "ldr r4, =gCurrentSceneData\n"
        "ldr r2, [r4]\n"
        "ldr r3, =0x175\n"
        "adds r2, r2, r3\n"
        "ldrb r1, [r2]\n"
        "adds r1, #1\n"
        "strb r1, [r2]\n"
        "ldr r1, [r4]\n"
        "adds r2, r1, r3\n"
        "ldrb r1, [r2]\n"
        "cmp r1, #4\n"
        "bls 1f\n"
        "movs r1, #4\n"
        "strb r1, [r2]\n"
        "1:\n"
        "pop {r4}\n"
        "pop {r1}\n"
        "bx r1\n"
        ".balign 4, 0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
