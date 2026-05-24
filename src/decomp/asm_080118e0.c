#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/scenes/main_menu.h"

extern void scene_set_current_thread(u32);
extern void func_080117A8(s32);
extern void func_08011864(u8);
extern u32 D_083FBBF8;

void func_080118E0(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");

    scene_set_current_thread(0);
    r4 = (u32)&D_03006518;
    r0 = *(u8 *)(r4 + 2);
    func_080117A8(r0);
    r0 = *(u8 *)(r4 + 2);
    func_08011864(r0);
    r0 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r0;
    r1 += 0xDD;
    r2 = *(u8 *)r1;
    r0 = 2;
    r0 = -r0;
    r0 &= r2;
    *(u8 *)r1 = r0;
    r0 = (u32)&D_083FBBF8;
    asm volatile("bl play_sound" :: "r"(r0) : "r0", "r1", "r2", "r3", "lr", "memory");
}
#endif
