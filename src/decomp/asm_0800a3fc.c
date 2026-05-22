#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *D_083ADADC;
extern u32 func_0800430C(u32, void *, u32, u32);
extern void func_0800D23C(void);
extern u32 get_current_mem_id(void);

__attribute__((naked))
u32 func_0800A3FC(u32 arg0, u32 arg1) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, r5, lr}\n"
        "adds r4, r0, #0\n"
        "adds r5, r1, #0\n"
        "lsls r4, r4, #0x10\n"
        "lsrs r4, r4, #0x10\n"
        "lsls r5, r5, #0x18\n"
        "lsrs r5, r5, #0x18\n"
        "bl get_current_mem_id\n"
        "lsls r0, r0, #0x10\n"
        "lsrs r0, r0, #0x10\n"
        "ldr r1, =D_083ADADC\n"
        "adds r2, r4, #0\n"
        "adds r3, r5, #0\n"
        "bl func_0800430C\n"
        "adds r4, r0, #0\n"
        "bl func_0800D23C\n"
        "adds r0, r4, #0\n"
        "pop {r4, r5}\n"
        "pop {r1}\n"
        "bx r1\n"
        ".balign 4, 0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
