#include "global.h"

extern void func_08006F84(s16, s16 *, s16 *);
extern void *func_0800C1C0(s16, u16, s16, s16, s16, s16, s16);
typedef void *(*Func0800C1C0Raw)(s16, s16, s16, s32, s32, s32, s32);

void func_0800C218(s16 arg0, u16 arg1, u16 arg2, u16 arg3, s32 arg4) {
    s16 spC;
    s16 spE;
    s16 temp_r2;

    temp_r2 = arg0;
    func_08006F84(temp_r2, &spC, &spE);
    ((Func0800C1C0Raw)func_0800C1C0)(temp_r2, spC, spE, (s32)(u16)arg1,
                                     (s32)(s16)arg2, (s32)(s16)arg3,
                                     (s32)(s16)(u16)arg4);
}
