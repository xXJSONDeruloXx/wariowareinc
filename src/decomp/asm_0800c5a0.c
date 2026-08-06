#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_08006F84(s16, s16 *, s16 *);
extern void *func_0800C548(s16, u16, s16, s16, s16, s16, s16);
typedef void *(*Func0800C548Raw)(s16, s16, s16, s16, s32, s32, s32);

u32 func_0800C5A0(s16 arg0, u16 arg1, u16 arg2, u16 arg3, s32 arg4) {
    s16 spC;
    s16 spE;
    s32 sp10;
    s16 temp_r0;

    temp_r0 = arg0;
    sp10 = (s32)(u16)arg4;
    func_08006F84(temp_r0, &spC, &spE);
    ((Func0800C548Raw)func_0800C548)(temp_r0, spC, spE, (s16)arg1,
                                     (s32)(s16)arg2, (s32)(s16)arg3, sp10);
}
#endif
