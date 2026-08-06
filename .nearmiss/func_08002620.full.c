#include "global.h"

extern u8 math_sqrt_rom[];
extern u8 fast_udivsi3_rom[];
extern u8 D_030001C0[];
extern u32 D_03003FE4;

void func_08002620(void) {
    register u32 r2 asm("r2") = (u32)&REG_DMA3SAD;
    register u32 r1 asm("r1") = (u32)math_sqrt_rom;
    register u32 r3 asm("r3");
    register u32 r0 asm("r0");

    *(volatile u32 *)r2 = r1;
    r3 = (u32)D_030001C0;
    *(volatile u32 *)(r2 + 4) = r3;
    r0 = (u32)fast_udivsi3_rom;
    r0 -= r1;
    r0 >>= 2;
    r1 = 0x84;
    r1 <<= 24;
    r0 |= r1;
    *(volatile u32 *)(r2 + 8) = r0;
    r0 = *(volatile u32 *)(r2 + 8);
    *(u32 *)&D_03003FE4 = r3;
}
