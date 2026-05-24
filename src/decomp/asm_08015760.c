#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"
#include "include/scenes.h"

extern u8 D_083AA294;

extern void scene_set_current_thread(u32);
extern void sprite_set_x_y(void *, s32, s32, s32);

void func_08015760(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");

    r0 = 0;
    scene_set_current_thread(r0);

    r4 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r4;
    r1 = 0xE2;
    r1 <<= 1;
    r0 = r0 + r1;
    r2 = 0;
    r0 = *(s16 *)(r0 + r2);
    r0 <<= 4;
    r1 = (u32)&D_083AA294;
    r0 = r0 + r1;
    r3 = *(u32 *)(r0 + 0xC);
    r0 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r0;
    r1 = (u32)&gCurrentSceneSpritePool;
    r1 = *(u32 *)r1;
    r2 = 8;
    r1 = *(s16 *)(r1 + r2);
    r2 = *(u16 *)r3;
    r2 += 0x4E;
    r2 <<= 16;
    r2 = (u32)((s32)r2 >> 16);
    r3 = *(u16 *)(r3 + 2);
    r3 += 6;
    r3 <<= 16;
    r3 = (u32)((s32)r3 >> 16);
    sprite_set_x_y((void *)r0, r1, r2, r3);

    r1 = *(u32 *)r4;
    r1 += 0xDD;
    r2 = *(u8 *)r1;
    r0 = 2;
    r0 = -r0;
    r0 = r2 & r0;
    *(u8 *)r1 = r0;
}
#endif
