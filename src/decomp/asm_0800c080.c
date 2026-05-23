#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 D_083A4A80;

__attribute__((naked)) u32 func_0800C080(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, r5, r6, lr}\n"
        "sub sp, #0x10\n"
        "ldr r5, [sp, #0x20]\n"
        "lsls r1, r1, #0x10\n"
        "lsrs r1, r1, #0x10\n"
        "add r4, sp, #4\n"
        "movs r6, #0\n"
        "strh r0, [r4, #0]\n"
        "adds r0, r4, #0\n"
        "strh r1, [r0, #2]\n"
        "strh r2, [r0, #4]\n"
        "strh r3, [r0, #6]\n"
        "strh r5, [r0, #8]\n"
        "bl get_current_mem_id\n"
        "lsls r0, r0, #0x10\n"
        "lsrs r0, r0, #0x10\n"
        "ldr r1, =D_083A4A80\n"
        "str r6, [sp, #0]\n"
        "add r2, sp, #4\n"
        "movs r3, #0\n"
        "bl start_new_task\n"
        "add sp, #0x10\n"
        "pop {r4, r5, r6}\n"
        "pop {r1}\n"
        "bx r1\n"
        ".balign 4, 0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
