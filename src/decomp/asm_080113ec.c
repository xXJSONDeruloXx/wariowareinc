#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"
#include "scenes.h"

// Forward declarations
extern void func_080122FC(void);
extern void func_08013188(void);

__attribute__((naked))
void func_080113EC(void) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, lr}\n"
        "ldr r4, =gCurrentSceneData\n"
        "ldr r0, [r4]\n"
        "adds r0, #0xDD\n"
        "ldrb r1, [r0]\n"
        "lsls r0, r1, #0x1E\n"
        "cmp r0, #0\n"
        "blt _08011438\n"
        "lsls r0, r1, #0x1C\n"
        "cmp r0, #0\n"
        "blt _08011438\n"
        "lsls r0, r1, #0x1D\n"
        "cmp r0, #0\n"
        "bge _0801141A\n"
        "bl func_080122FC\n"
        "ldr r0, [r4]\n"
        "adds r0, #0xDD\n"
        "ldrb r2, [r0]\n"
        "movs r1, #5\n"
        "rsbs r1, r1, #0\n"
        "ands r1, r2\n"
        "strb r1, [r0]\n"
        "_0801141A:\n"
        "ldr r0, [r4]\n"
        "adds r0, #0xDD\n"
        "ldrb r0, [r0]\n"
        "lsls r0, r0, #0x1B\n"
        "cmp r0, #0\n"
        "bge _08011438\n"
        "bl func_08013188\n"
        "ldr r0, [r4]\n"
        "adds r0, #0xDD\n"
        "ldrb r2, [r0]\n"
        "movs r1, #0x11\n"
        "rsbs r1, r1, #0\n"
        "ands r1, r2\n"
        "strb r1, [r0]\n"
        "_08011438:\n"
        "pop {r4}\n"
        "pop {r0}\n"
        "bx r0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
