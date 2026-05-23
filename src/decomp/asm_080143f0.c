#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);
extern void func_080117FC(void);
extern void func_08015C38(void);
extern void func_08011730(u32);
extern struct Unk03006518 D_03006518;

void func_080143F0(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");

    scene_set_current_thread(0);
    r1 = (u32)&D_03006518;
    r0 = 0;
    ((u8 *)r1)[1] = r0;
    func_080117FC();
    func_08015C38();
    func_08011730(1);
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
