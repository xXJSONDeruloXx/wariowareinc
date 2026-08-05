#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_0800BF0C(s32 arg0);
extern void func_0800BF20(s32 arg0);
extern void func_0800BF34(s32 arg0, s32 arg1, s32 arg2);
extern void func_0800BF44(u32 arg0, u32 arg1, u32 arg2, u32 arg3);

void func_0800BF7C(s32 arg0, u32 arg1, s16 arg2, s16 arg3,
                   u32 arg4, u32 arg5, u32 arg6) {
    func_0800BF34(arg0, arg2, arg3);
    func_0800BF44(arg0, arg4, arg5, arg6);
    if (arg1 != 0) {
        func_0800BF0C(arg0);
    } else {
        func_0800BF20(arg0);
    }
}
#endif
