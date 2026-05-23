#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void func_0800C7A4(s32);
extern void func_08014CF8(void);
extern struct Unk03006518 D_03006518;

void func_08014DFC(u32 arg0, u32 arg1) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1") = arg1;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r3 = (u32)&D_03006518;
    r4 = 0;
    r2 = 6;
    ((u8 *)r3)[1] = r2;
    r2 = (u32)&gCurrentSceneData;
    r3 = *(u32 *)r2;
    r5 = 0xB8;
    r5 <<= 1;
    r2 = r3 + r5;
    *(u32 *)r2 = r1;
    r2 = 0xBC;
    r2 <<= 1;
    r1 = r3 + r2;
    *(u32 *)r1 = r0;
    asm volatile("add r5, #4" : "+r"(r5));
    r0 = r3 + r5;
    *(u32 *)r0 = r4;
    func_0800C7A4(0);
    func_08014CF8();
}
#endif
