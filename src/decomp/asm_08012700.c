#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/scenes/main_menu.h"

extern void func_08012658(void);
extern u8 D_083AA0C4[];
extern u32 D_083FBB1C;

void func_08012700(u32 arg0) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r2 = (u32)&D_03006518;
    r3 = 0;
    asm volatile("" : "+r"(r2));
    *(u8 *)r2 = r0;
    r0 <<= 4;
    r1 = (u32)D_083AA0C4;
    r0 += r1;
    r1 = *(u32 *)(r0 + 0xC);
    *(u8 *)(r2 + 4) = r3;
    *(u8 *)(r2 + 3) = r3;
    r0 = *(u8 *)(r2 + 1);
    if (r0 != 1) goto else_branch;
    r2 = 0;
    r0 = *(s16 *)(r1 + r2);
    r2 = 2;
    r1 = *(s16 *)(r1 + r2);
    r2 = (u32)func_08012658 + 1;
    asm volatile("bl func_08011504" :: "r"(r0), "r"(r1), "r"(r2), "r"(r3) : "r0", "r1", "r2", "r3", "lr", "memory");
    goto after;
else_branch:
    func_08012658();
after:
    r0 = (u32)&D_083FBB1C;
    asm volatile("bl play_sound" :: "r"(r0) : "r0", "r1", "r2", "r3", "lr", "memory");
}
#endif
