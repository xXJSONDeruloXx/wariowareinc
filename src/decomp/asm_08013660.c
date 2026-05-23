#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);
extern void func_08013B94(void);
extern void func_08013AF4(void);
extern void func_08013C60(void);

void func_08013660(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    scene_set_current_thread(0);
    func_08013B94();
    r5 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r5;
    r0 += 0xDD;
    r0 = *(u8 *)r0;
    r0 <<= 0x19;
    r4 = r0 >> 0x1F;
    if (r4 == 0) {
        func_08013AF4();
        func_08013C60();
        r0 = *(u32 *)r5;
        r0 += 0xF1;
        *(u8 *)r0 = r4;
    }
    r0 = *(u32 *)r5;
    r0 += 0xDD;
    r2 = *(u8 *)r0;
    r1 = 2;
    r1 = -r1;
    r1 &= r2;
    *(u8 *)r0 = r1;
}
#endif
