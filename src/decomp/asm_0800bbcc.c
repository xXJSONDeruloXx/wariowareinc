#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern void *gCurrentSceneData;
extern void func_0800B828(u32, u32);
extern void func_0800BA78(void);

void func_0800BBCC(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1") = (u32)arg1;
    register u32 r2 asm("r2") = (u32)arg2;
    register u32 r3 asm("r3") = (u32)arg3;
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r4 = r3;
    asm volatile("ldr %0, [sp, #0xC]" : "=r"(r5));
    asm volatile("" : "+r"(r0), "+r"(r1), "+r"(r2), "+r"(r4), "+r"(r5));
    r4 <<= 16;
    r4 >>= 16;
    r5 <<= 16;
    r5 >>= 16;
    asm volatile("bl func_0800B828" :: "r"(r0), "r"(r1), "r"(r2) : "r0", "r1", "r2", "r3", "lr", "memory");
    r0 = (u32)&gCurrentSceneData;
    r2 = *(u32 *)r0;
    r0 = 0xC0;
    r0 <<= 1;
    r1 = r2 + r0;
    r3 = 0xCC;
    r3 <<= 1;
    r0 = r2 + r3;
    *(u32 *)r1 = r0;
    r1 = 0xC2;
    r1 <<= 1;
    r0 = r2 + r1;
    *(u16 *)r0 = r4;
    r3 -= 0x12;
    r0 = r2 + r3;
    *(u16 *)r0 = r5;
    asm volatile("bl func_0800BA78" ::: "r0", "r1", "r2", "r3", "lr", "memory");
}
#endif
