#include "src/scenes/main_menu.h"
#include "src/code_08000f10.h"

extern void func_08012278(u32, u32, u32);
extern void func_0800A240(void *, u32, u32, u32);
extern u8 D_083A4A1C[];

void func_080122FC(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");

    dma3_fill(0, D_03004154, 0x20, 0x20, 0x100);
    r4 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r4;
    r0 = *(u32 *)(r0 + 0x78);
    r0 = *(u32 *)(r0 + 0x14);
    r1 = 0;
    r2 = 0xB;
    func_08012278(r0, r1, r2);
    r0 = (u32)D_083A4A1C;
    r1 = *(u32 *)r4;
    r1 = *(u32 *)(r1 + 0x78);
    r2 = 0;
    r3 = 0;
    func_0800A240((void *)r0, r1, r2, r3);
    r1 = *(u32 *)r4;
    r1 += 0xDD;
    r0 = *(u8 *)r1;
    r2 = 2;
    r0 |= r2;
    *(u8 *)r1 = r0;
}
