#include "global.h"
#include "types.h"

extern void *func_08006184(u32, u32);

void *func_08005F64(u32 arg0, u32 arg1, u32 arg2) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r4 asm("r4") = arg0;
    register u32 r5 asm("r5");
    register u32 r6 asm("r6") = arg1;
    register u32 r8 asm("r8") = arg2;

    r4 <<= 16;
    r4 >>= 16;
    r0 = r4;
    r1 = 8;
    r5 = (u32)func_08006184(r0, r1);
    r1 = r6 * r8;
    r1 <<= 1;
    r0 = r4;
    r0 = (u32)func_08006184(r0, r1);
    *(u32 *)r5 = r0;
    *(u16 *)(r5 + 4) = r6;
    r0 = r8;
    *(u16 *)(r5 + 6) = r0;
    r0 = r5;
    return (void *)r0;
}
