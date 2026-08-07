#include "global.h"

extern void func_08006F84(s16, s16 *, s16 *);
extern void *func_0800C344(s16, u8, s16, s16, s16, s16, s16, s16, s16);

void *func_0800C3AC(s16 arg0, u8 arg1, u16 arg2, u16 arg3, s32 arg4,
                    s32 arg5) {
    s16 x;
    s16 y;
    u32 arg1u8;
    u32 arg2u16;
    u32 arg3u16;
    u32 arg4u16;
    u32 arg5u16;

    arg1u8 = (u8)arg1;
    arg2u16 = (u16)arg2;
    arg3u16 = (u16)arg3;
    arg4u16 = (u16)arg4;
    arg5u16 = (u16)arg5;
    func_08006F84(arg0, &x, &y);
    return func_0800C344(arg0, (u8)arg1u8, x, y, 0, (s16)arg2u16,
                         (s16)arg3u16, (s16)arg4u16, (s16)arg5u16);
}
