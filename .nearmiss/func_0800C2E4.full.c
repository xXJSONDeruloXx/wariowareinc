#include "global.h"

extern void func_08006F84(s16, s16 *, s16 *);
extern void *func_0800C298(s16, u16, s16, s16, s16, s16);
typedef void *(*Func0800C298Raw)(s16, s16, s16, s16, s32, s32);

u32 func_0800C2E4(s16 arg0, u16 arg1, u16 arg2, u16 arg3) {
    s16 sp8;
    s16 spA;
    s16 temp_r0;

    temp_r0 = arg0;
    func_08006F84(temp_r0, &sp8, &spA);
    ((Func0800C298Raw)func_0800C298)(temp_r0, sp8, spA, (s16)arg1,
                                     (s32)(s16)arg2, (s32)arg3);
}
