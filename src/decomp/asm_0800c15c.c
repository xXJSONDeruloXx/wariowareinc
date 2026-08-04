#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_08006F84(s16, s16 *, s16 *);
extern void *func_0800C110();

u32 func_0800C15C(s16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    s16 sp8;
    s16 spA;
    s16 temp_r8;

    temp_r8 = arg0;
    func_08006F84(temp_r8, &sp8, &spA);
    func_0800C110(temp_r8, sp8, spA, (s16)arg1, (s16)arg2, (s16)arg3);
}

__attribute__((section(".text"))) const u16 _padding_0800c15c = 0;
#endif
