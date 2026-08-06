#include "global.h"

extern u32 D_03000E80;
extern u32 D_03000E88;
extern u32 D_03000E90;

void func_080F2894(s32 arg0) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1") = (u32)&D_03000E80;
    register u32 r2 asm("r2");

    r1 += r0;
    r2 = 1;
    *(u8 *)r1 = r2;
    r1 = (u32)&D_03000E88;
    r0 <<= 1;
    r1 += r0;
    r2 = 0xFFFF;
    *(u16 *)r1 = r2;
    r1 = (u32)&D_03000E90;
    r0 += r1;
    r1 = 1;
    r1 = -r1;
    *(u16 *)r0 = r1;
}
