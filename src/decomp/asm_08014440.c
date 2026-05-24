#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern struct Unk03006518 D_03006518;

extern void scene_set_current_thread(u32);
extern void set_pause_beatscript_scene(u32);
extern void func_0800C7A4(s32);

void func_08014440(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r0 = 0;
    scene_set_current_thread(r0);

    r5 = (u32)&gCurrentSceneData;
    r2 = *(u32 *)r5;
    r1 = 0xA6;
    r1 <<= 1;
    r0 = r2 + r1;
    r0 = *(u8 *)r0;

    if (r0 == 0) goto else_branch;

    r1 = (u32)&D_03006518;
    r0 = 4;
    *(u8 *)(r1 + 1) = r0;
    goto done;

else_branch:
    r1 = (u32)&D_03006518;
    r0 = 9;
    *(u8 *)(r1 + 1) = r0;
    r4 = 0;
    r0 = 3;
    *(u16 *)(r2 + 0x38) = r0;
    r0 = 0;
    set_pause_beatscript_scene(r0);
    r0 = *(u32 *)r5;
    *(u8 *)(r0 + 8) = r4;
    r0 = 0;
    func_0800C7A4(r0);

done:
    return;
}
#endif
