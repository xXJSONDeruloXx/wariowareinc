#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "types.h"
#include "src/lib_sprite.h"
#include "src/scenes/main_menu.h"

extern u32 func_08005920(void *);
extern void func_08005834(void *);

void func_08011584(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r5 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r5;
    r1 = 0xD6;
    r1 <<= 1;
    r0 += r1;
    r4 = *(u32 *)r0;
    r0 = r4;
    asm volatile("bl func_08005920" : "=r"(r0) : "r"(r0) : "r1", "r2", "r3", "lr", "memory");
    if (r0 != 1) goto done;
    r0 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r0;
    r1 = (u32)&gCurrentSceneSpritePool;
    r1 = *(u32 *)r1;
    r2 = 0;
    r1 = *(s16 *)(r1 + r2);
    r3 = *(u32 *)r5;
    r5 = 0xD8;
    r5 <<= 1;
    r2 = r3 + r5;
    r5 = 0;
    r2 = *(s16 *)(r2 + r5);
    r5 = 0xD9;
    r5 <<= 1;
    r3 += r5;
    r5 = 0;
    r3 = *(s16 *)(r3 + r5);
    asm volatile("bl sprite_set_x_y" :: "r"(r0), "r"(r1), "r"(r2), "r"(r3) : "r0", "r1", "r2", "r3", "lr", "memory");
    r0 = r4;
    asm volatile("bl func_08005834" :: "r"(r0) : "r0", "r1", "r2", "r3", "lr", "memory");
done:;
}
#endif
