#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void func_0800B828(u32, u32);
extern void func_0800BA78(void);
extern u32 D_083ADADC;

void func_0800BB74(void *arg0) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r1 = (u32)&D_083ADADC;
    r2 = 0;
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
    r0 = 0xC2;
    r0 <<= 1;
    r1 = r2 + r0;
    r0 = 0x78;
    *(u16 *)r1 = r0;
    r3 -= 0x12;
    r1 = r2 + r3;
    r0 = 0x40;
    *(u16 *)r1 = r0;
    func_0800BA78();
}
#endif
