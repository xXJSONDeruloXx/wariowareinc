#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"
#include "include/scenes.h"

void func_08014810(u32 arg0) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4") = arg0;
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r5 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r5;
    r6 = (u32)&gCurrentSceneSpritePool;
    r1 = *(u32 *)r6;
    r2 = 0x2C;
    r1 = *(s16 *)(r1 + r2);
    r2 = 6;
    asm volatile("bl sprite_set_base_palette" :: "r"(r0), "r"(r1), "r"(r2) : "r0", "r1", "r2", "r3", "lr", "memory");

    r0 = *(u32 *)r5;
    r1 = *(u32 *)r6;
    r2 = 0x2E;
    r1 = *(s16 *)(r1 + r2);
    r2 = 6;
    asm volatile("bl sprite_set_base_palette" :: "r"(r0), "r"(r1), "r"(r2) : "r0", "r1", "r2", "r3", "lr", "memory");

    if (r4 == 0) goto done;

    r0 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r0;
    r1 = 0xA6;
    r1 <<= 1;
    r0 = r0 + r1;
    r0 = *(u8 *)r0;
    if (r0 == 0) goto else_branch;

    r0 = *(u32 *)r5;
    r1 = *(u32 *)r6;
    r2 = 0x2E;
    r1 = *(s16 *)(r1 + r2);
    r2 = 0xC;
    asm volatile("bl sprite_set_base_palette" :: "r"(r0), "r"(r1), "r"(r2) : "r0", "r1", "r2", "r3", "lr", "memory");
    goto done;

else_branch:
    r0 = *(u32 *)r5;
    r1 = *(u32 *)r6;
    r2 = 0x2C;
    r1 = *(s16 *)(r1 + r2);
    r2 = 0xC;
    asm volatile("bl sprite_set_base_palette" :: "r"(r0), "r"(r1), "r"(r2) : "r0", "r1", "r2", "r3", "lr", "memory");

done:
    return;
}
#endif
