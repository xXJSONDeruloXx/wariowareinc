#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern void *gCurrentSceneData;
extern void scene_set_current_thread(u32);
extern void func_0800BF0C(s32);

void func_08013EC0(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");

    r0 = 0;
    scene_set_current_thread(r0);

    r3 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r3;
    r1 += 0xDE;
    r2 = *(u8 *)r1;
    r0 = 2;
    r0 = -r0;
    r0 = r2 & r0;
    *(u8 *)r1 = r0;

    r1 = *(u32 *)r3;
    r1 += 0xDE;
    r0 = *(u8 *)r1;
    r2 = 4;
    r0 = r0 | r2;
    *(u8 *)r1 = r0;
    asm volatile("" : "+r"(r2));

    r0 = *(u32 *)r3;
    r0 += 0xFE;
    r4 = 0;
    *(u8 *)r0 = r4;

    r1 = *(u32 *)r3;
    r2 = 0x80;
    r2 <<= 1;
    r0 = r1 + r2;
    *(u16 *)r0 = r2;
    r0 = 0x81;
    r0 <<= 1;
    r2 = r1 + r0;
    r0 = 0xA0;
    *(u16 *)r2 = r0;
    r0 += 0x64;
    r1 = r1 + r0;
    *(u16 *)r1 = r4;

    r0 = 2;
    func_0800BF0C(r0);
}
#endif
