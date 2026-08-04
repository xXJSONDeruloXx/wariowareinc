#include "global.h"

extern void func_080F2704(u32, s16);

void func_080F2F68(u32 *arg0, s32 unused, s16 arg2) {
    (void)unused;
    func_080F2704(arg0[1], arg2);
}
