#include "global.h"

u32 func_080F282C(u16 arg0) {
    register u32 r0 asm("r0") = arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r0 <<= 16;
    r0 >>= 16;
    asm("" ::: "memory");
    r2 = 0x03000E78;
    r3 = *(u16 *)r2;
    r1 = 0x6D;
    r1 *= r3;
    r3 = 0x3FD;
    r1 += r3;
    *(u16 *)r2 = r1;
    r1 = *(u16 *)r2;
    r0 *= r1;
    r0 >>= 16;
    return r0;
}
