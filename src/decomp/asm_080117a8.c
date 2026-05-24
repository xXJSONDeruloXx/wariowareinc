#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void func_08011774(void);
extern void func_0800C77C(u32);
extern u8 D_083A9CE0;

void func_080117A8(s32 arg0) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r4 = (u32)arg0;
    func_08011774();

    r6 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r6;
    r5 = (u32)&gCurrentSceneSpritePool;
    r2 = *(u32 *)r5;
    r1 = r4 << 1;
    r1 = r1 + r2;
    r2 = 2;
    r1 = *(s16 *)(r1 + r2);
    r2 = 0;
    asm volatile("bl sprite_set_anim_cel" :: "r"(r0), "r"(r1), "r"(r2) : "r0", "r1", "r2", "r3", "lr", "memory");

    r0 = (u32)&D_083A9CE0;
    r4 = r4 << 2;
    r4 = r4 + r0;
    r3 = *(u32 *)r4;

    r0 = *(u32 *)r6;
    r1 = *(u32 *)r5;
    r4 = 0x14;
    r1 = *(s16 *)(r1 + r4);
    r4 = 0;
    r2 = *(s16 *)(r3 + r4);
    r4 = 2;
    r3 = *(s16 *)(r3 + r4);
    asm volatile("bl sprite_set_x_y" :: "r"(r0), "r"(r1), "r"(r2), "r"(r3) : "r0", "r1", "r2", "r3", "lr", "memory");

    r0 = 0xA;
    func_0800C77C(r0);
}
#endif
