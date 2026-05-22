#include "global.h"

void func_080CD564(void *arg0, void *arg1) {
    asm volatile(
        ".syntax unified\n"
        "adds r3, r0, #0\n"
        "ldr r2, [r1, #0x28]\n"
        "str r2, [r3, #0x28]\n"
        "ldr r1, [r1, #0x2c]\n"
        "str r1, [r3, #0x2c]\n"
        ".syntax divided\n"
    );
}
