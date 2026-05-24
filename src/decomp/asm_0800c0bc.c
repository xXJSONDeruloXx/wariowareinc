#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_08006F84(s16, s16 *, s16 *);
extern void *func_0800C080(s16, s16, s16, s16, s16);

u32 func_0800C0BC(s16 arg0, u16 arg1, u16 arg2) {
    s16 sp4;
    s16 sp6;
    s16 temp_r6;

    temp_r6 = arg0;
    func_08006F84(temp_r6, &sp4, &sp6);
    func_0800C080(temp_r6, sp4, sp6, (s16)arg1, (s16)arg2);
}
#endif
