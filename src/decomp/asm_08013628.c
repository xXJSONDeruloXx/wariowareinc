#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"

// D_083AAD70 is an array of pointers
extern u32 D_083AAD70[];

__attribute__((naked))
u8 func_08013628(void) {
    asm volatile(
        ".syntax unified\n"
        "ldr r0, =D_083AAD70\n"
        "ldr r2, =D_03006518\n"
        "ldrb r1, [r2]\n"
        "lsls r1, r1, #2\n"
        "adds r1, r0\n"
        "ldrb r0, [r2, #3]\n"
        "lsls r0, r0, #2\n"
        "ldrb r2, [r2, #4]\n"
        "adds r0, r2\n"
        "ldr r1, [r1]\n"
        "lsls r0, r0, #3\n"
        "adds r0, r1\n"
        "ldrb r0, [r0]\n"
        "bx lr\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
