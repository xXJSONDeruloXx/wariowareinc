#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *func_0800C110();

u32 func_0800C15C(s16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    volatile s16 sp8;
    volatile s16 spA;
    s16 temp_r8;
    register s16 *sb asm("r9");
    register s32 r0 asm("r0");
    register s32 genX asm("r1");
    register s32 genY asm("r2");

    temp_r8 = arg0;
    r0 = temp_r8;
    asm volatile(
        "movs r2, #0xA\n"
        "add r2, sp\n"
        "mov r9, r2\n"
        "add r1, sp, #8\n"
        "bl func_08006F84"
        : "+r"(r0), "=r"(sb)
        :
        : "r1", "r2", "r3", "lr", "memory");
    asm volatile(
        "add r0, sp, #8\n"
        "movs r3, #0\n"
        "ldrsh r1, [r0, r3]\n"
        "mov r0, r9\n"
        "movs r3, #0\n"
        "ldrsh r2, [r0, r3]"
        : "=r"(genX), "=r"(genY)
        : "r"(sb)
        : "r0", "r3", "memory");
    func_0800C110(temp_r8, genX, genY, (s16)arg1, (s16)arg2, (s16)arg3);
}

__attribute__((section(".text"))) const u16 _padding_0800c15c = 0;
#endif
