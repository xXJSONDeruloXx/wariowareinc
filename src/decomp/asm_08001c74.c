#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern s16 gCosineTable[];
extern s16 gSineTable[];
extern s32 __divsi3(s32, s32);

void func_08001C74(void *out, s32 angle, u32 idx) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1") = (u32)angle;
    register u32 r2 asm("r2") = idx;
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5") = (u32)out;

    r4 = r2;
    asm volatile("" : "+r"(r4));
    r4 <<= 16;
    r4 >>= 16;
    r1 <<= 16;
    r1 = (u32)((s32)r1 >> 16);
    r0 = 0x80;
    r0 <<= 9;
    r0 = __divsi3((s32)r0, (s32)r1);

    r2 = 0xFF;
    r2 &= r4;
    r3 = (u32)gCosineTable;
    r2 <<= 1;
    r3 = r2 + r3;
    r4 = 0;
    r1 = *(s16 *)(r3 + r4);
    r1 = (u32)((s32)r1 * (s32)r0);
    r1 = (u32)((s32)r1 >> 8);
    *(u16 *)r5 = r1;

    r1 = (u32)gSineTable;
    r2 += r1;
    r4 = 0;
    r1 = *(s16 *)(r2 + r4);
    r1 = (u32)(-(s32)r1);
    r1 = (u32)((s32)r1 * (s32)r0);
    r1 = (u32)((s32)r1 >> 8);
    *(u16 *)(r5 + 2) = r1;

    r4 = 0;
    r1 = *(s16 *)(r2 + r4);
    r1 = (u32)((s32)r1 * (s32)r0);
    r1 = (u32)((s32)r1 >> 8);
    *(u16 *)(r5 + 4) = r1;

    r2 = 0;
    r1 = *(s16 *)(r3 + r2);
    r0 = (u32)((s32)r1 * (s32)r0);
    r0 = (u32)((s32)r0 >> 8);
    *(u16 *)(r5 + 6) = r0;
}
#endif
