#include "global.h"

extern void func_08006F84(s16, s16 *, s16 *);
extern void *func_0800C430(s16, s32, s16, s16, s32, s32, s32);

void func_0800C4E0(s16 arg0, s32 arg1, u16 arg2, u16 arg3, s32 arg4) {
    s16 spC;
    s16 spE;
    s16 temp_r0;

    temp_r0 = arg0;
    func_08006F84(temp_r0, &spC, &spE);
    func_0800C430(temp_r0, arg1, spC, spE, (s32)(s16)arg2,
                  (s32)(s16)arg3, (s32)(u16)arg4);
}
