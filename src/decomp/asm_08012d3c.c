#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern struct Unk03006518 D_03006518;
extern void *gCurrentSceneData;
extern void scene_set_current_thread(u32);
extern void func_08012EC4(u8);
extern void func_08012CC8(void);

void func_08012D3C(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");

    r0 = 0;
    asm volatile("bl scene_set_current_thread" :: "r"(r0) : "r0", "r1", "r2", "r3", "lr", "memory");
    r0 = (u32)&D_03006518;
    r0 = *(u8 *)r0;
    asm volatile("bl func_08012EC4" :: "r"(r0) : "r0", "r1", "r2", "r3", "lr", "memory");
    r4 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r4;
    r0 += 0xDD;
    r0 = *(u8 *)r0;
    r0 <<= 28;
    if ((s32)r0 < 0) goto call_clear;
    asm volatile("bl func_08012CC8" ::: "r0", "r1", "r2", "r3", "lr", "memory");
call_clear:
    r0 = *(u32 *)r4;
    r0 += 0xDD;
    r2 = *(u8 *)r0;
    r1 = 0x21;
    r1 = -r1;
    r1 &= r2;
    *(u8 *)r0 = r1;
}
#endif
