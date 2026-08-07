#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_08006F84(s16, s16 *, s16 *);
extern void *func_0800C1C0(s16, u16, s16, s16, s16, s16, s16);
typedef void *(*Func0800C1C0Raw)(s16, s16, s16, s16, s16, s16, s16);

void *func_0800C218(s16 arg0, u16 arg1, u16 arg2, u16 arg3, s32 arg4) {
    s16 x;
    s16 y;
    u32 arg1u16;
    u32 arg2u16;
    u32 arg3u16;
    u32 arg4u16;

    arg1u16 = (u16)arg1;
    arg2u16 = (u16)arg2;
    arg3u16 = (u16)arg3;
    arg4u16 = (u16)arg4;
    func_08006F84(arg0, &x, &y);
    return ((Func0800C1C0Raw)func_0800C1C0)(
        arg0, x, y, (s16)arg1u16, (s16)arg2u16, (s16)arg3u16,
        (s16)arg4u16);
}
#endif
