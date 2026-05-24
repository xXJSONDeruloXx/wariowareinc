#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "scenes.h"
#include "src/lib_sprite.h"

extern void scene_set_current_thread(u32);
extern struct Unk03006518 D_03006518;
extern u8 D_083AA0C4[];
extern void func_08012420(u32);
extern void func_08015C38(void);
extern u32 func_08012C18(u32);
extern void func_08015A88(void);

void func_08012658(void) {
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
    r1 = (u32)D_083AA0C4;
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
    func_08012420(r0);

    r0 = *(u8 *)(r4 + 1);
    if (r0 != 1) goto skip;
    func_08015C38();
    r0 = *(u8 *)r4;
    func_08012C18(r0);
    func_08015A88();

skip:
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
