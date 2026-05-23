#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "types.h"
#include "src/scenes/main_menu.h"

extern void scene_set_current_thread(u32);
extern void func_08011824(void);
extern u32 func_08012C18(u32);
extern void func_08015A88(void);
extern void func_08012C80(u32);

void func_08011920(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");

    scene_set_current_thread(0);
    r4 = (u32)&D_03006518;
    r0 = 1;
    *(u8 *)(r4 + 1) = r0;
    func_08011824();
    r0 = *(u8 *)r4;
    r0 = func_08012C18(r0);
    func_08015A88();
    r0 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r0;
    asm volatile("" : "+r"(r0));
    r1 = r0;
    r1 += 0x88;
    r0 = *(u8 *)r1;
    r0 <<= 0x1F;
    if (r0 == 0) goto set_default;
    r0 = *(u16 *)r1;
    r0 <<= 0x17;
    r0 >>= 0x18;
    if ((s32)r0 > 0x27) goto after;
set_default:
    r0 = *(u8 *)r4;
    func_08012C80(r0);
after:
    r0 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r0;
    r1 += 0xDD;
    r2 = *(u8 *)r1;
    r0 = 2;
    r0 = -r0;
    r0 &= r2;
    *(u8 *)r1 = r0;
}
#endif
