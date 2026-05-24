#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"
#include "include/scenes.h"

extern struct Unk03006518 D_03006518;
extern u8 D_083AA294;

extern void scene_set_current_thread(u32);
extern void sprite_set_x_y(void *, s32, s32, s32);
extern u32 func_080135E8(u32);
extern void func_08015A88(void);
extern void func_08012E04(void);

void func_08013388(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r0 = 0;
    scene_set_current_thread(r0);

    r4 = (u32)&D_03006518;
    r0 = *(u8 *)r4;
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
    r5 = 0;
    r2 = *(s16 *)(r3 + r5);
    r5 = 2;
    r3 = *(s16 *)(r3 + r5);
    asm volatile("bl sprite_set_x_y" :: "r"(r0), "r"(r1), "r"(r2), "r"(r3) : "r0", "r1", "r2", "r3", "lr", "memory");

    r0 = *(u8 *)r4;
    func_080135E8(r0);

    func_08015A88();

    func_08012E04();

    r0 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r0;
    r1 += 0xDD;
    r2 = *(u8 *)r1;
    r0 = 2;
    r0 = -r0;
    r0 = r2 & r0;
    *(u8 *)r1 = r0;
}
#endif
