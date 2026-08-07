#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_08006F84(s16 value, s16 *x, s16 *y);
extern void *func_0800C430(s16, s32, s16, s16, s32, s32, s32);

void *func_0800C4E0(s16 arg0, s32 arg1, u16 arg2, u16 arg3, s32 arg4) {
    s16 x;
    s16 y;
    u32 arg4u16;

    arg4u16 = (u16)arg4;
    func_08006F84(arg0, &x, &y);
    return func_0800C430(arg0, arg1, x, y, (s32)(s16)arg2, (s32)(s16)arg3,
                         arg4u16);
}
#endif
