#if __INCLUDE_LEVEL__ > 0
#include "global.h"

__attribute__((naked))
void func_080115DC(void) {
    asm volatile(
        ".syntax unified\n"
        "push {lr}\n"
        "sub sp, #4\n"
        "ldr r0, =gCurrentSceneData\n"
        "ldr r1, [r0]\n"
        "adds r0, r1, #0\n"
        "adds r0, #0xDC\n"
        "ldrb r0, [r0]\n"
        "cmp r0, #0\n"
        "beq 1f\n"
        "adds r0, r1, #0\n"
        "adds r0, #0xD4\n"
        "ldr r0, [r0]\n"
        "adds r1, #0xD8\n"
        "ldr r1, [r1]\n"
        "movs r2, #0xA0\n"
        "lsls r2, r2, #3\n"
        "movs r3, #0x80\n"
        "lsls r3, r3, #1\n"
        "str r3, [sp]\n"
        "movs r3, #0x20\n"
        "bl dma3_set\n"
        "1:\n"
        "add sp, #4\n"
        "pop {r0}\n"
        "bx r0\n"
        ".balign 4, 0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
