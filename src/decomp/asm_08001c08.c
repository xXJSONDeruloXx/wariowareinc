#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern u32 D_03000010[];
extern s16 gCosineTable[];
extern s16 gSineTable[];

void func_08001C08(s32 arg0, u16 arg1, u16 arg2, u16 arg3) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1") = (u32)arg1;
    register u32 r2 asm("r2") = (u32)arg2;
    register u32 r3 asm("r3") = (u32)arg3;
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r1 <<= 16;
    r5 = r1 >> 16;
    r2 <<= 16;
    r6 = r2 >> 16;
    r3 <<= 16;
    r3 >>= 16;
    if ((s32)r0 < 0) goto done;
    r4 = r0 << 3;
    r0 = (u32)D_03000010;
    r4 += r0;
    r1 = 0xFF;
    r1 &= r3;
    r2 = r5 << 16;
    r2 = (u32)((s32)r2 >> 16);
    r3 = (u32)gCosineTable;
    r1 <<= 1;
    r3 = r1 + r3;
    r5 = 0;
    r0 = (u32)*(s16 *)(r3 + r5);
    r0 = (u32)((s32)r2 * (s32)r0);
    r0 = (u32)((s32)r0 >> 8);
    *(u16 *)r4 = r0;
    r0 = (u32)gSineTable;
    r1 += r0;
    r5 = 0;
    r0 = (u32)*(s16 *)(r1 + r5);
    r0 = (u32)(-(s32)r0);
    r0 = (u32)((s32)r2 * (s32)r0);
    r0 = (u32)((s32)r0 >> 8);
    *(u16 *)(r4 + 2) = r0;
    r2 = r6 << 16;
    r2 = (u32)((s32)r2 >> 16);
    r5 = 0;
    r0 = (u32)*(s16 *)(r1 + r5);
    r0 = (u32)((s32)r2 * (s32)r0);
    r0 = (u32)((s32)r0 >> 8);
    *(u16 *)(r4 + 4) = r0;
    r1 = 0;
    r0 = (u32)*(s16 *)(r3 + r1);
    r0 = (u32)((s32)r2 * (s32)r0);
    r0 = (u32)((s32)r0 >> 8);
    *(u16 *)(r4 + 6) = r0;
done:;
}
#endif
