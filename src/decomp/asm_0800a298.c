#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/lib_sprite.h"

extern struct SpriteHandler *gSpriteHandler;
extern void *gCurrentSceneData;
extern void sprite_id_and_attr(struct SpriteHandler *, u32, u32);
extern void sprite_id_orr_attr(struct SpriteHandler *, u32, u32);

__attribute__((naked))
void func_0800A298(u32 arg0, u32 arg1) {
    asm volatile(
        ".syntax unified\n"
        "push {r4, r5, r6, lr}\n"
        "adds r5, r0, #0\n"
        "adds r6, r1, #0\n"
        "ldr r4, =gSpriteHandler\n"
        "ldr r0, [r4]\n"
        "mvns r2, r5\n"
        "movs r1, #1\n"
        "bl sprite_id_and_attr\n"
        "ldr r0, [r4]\n"
        "adds r2, r5, #0\n"
        "ands r2, r6\n"
        "movs r1, #1\n"
        "bl sprite_id_orr_attr\n"
        "ldr r0, =gCurrentSceneData\n"
        "ldr r1, [r0]\n"
        "movs r2, #0x9D\n"
        "lsls r2, r2, #2\n"
        "adds r0, r1, r2\n"
        "str r6, [r0]\n"
        "adds r2, #4\n"
        "adds r0, r1, r2\n"
        "str r5, [r0]\n"
        "pop {r4, r5, r6}\n"
        "pop {r0}\n"
        "bx r0\n"
        ".balign 4, 0\n"
        ".ltorg\n"
        ".syntax divided\n"
    );
}
#endif
