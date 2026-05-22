#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 get_current_mem_id(void);
extern void start_new_task(u32 memID, void *unk1, void *unk2, void *unk3, u32 stackArg);

__attribute__((naked))
void func_0800A240(void *arg0, void *arg1, void *arg2, u32 arg3) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, r5, r6, lr}\n"
        "mov r6, r8\n"
        "push {r6}\n"
        "sub sp, #4\n"
        "adds r5, r0, #0\n"
        "adds r6, r1, #0\n"
        "mov r8, r2\n"
        "adds r4, r3, #0\n"
        "bl get_current_mem_id\n"
        "lsls r0, r0, #0x10\n"
        "lsrs r0, r0, #0x10\n"
        "str r4, [sp]\n"
        "adds r1, r5, #0\n"
        "adds r2, r6, #0\n"
        "mov r3, r8\n"
        "bl start_new_task\n"
        "add sp, #4\n"
        "pop {r3}\n"
        "mov r8, r3\n"
        "pop {r4, r5, r6}\n"
        "pop {r1}\n"
        "bx r1\n"
        ".syntax divided\n"
    );
}
#endif
