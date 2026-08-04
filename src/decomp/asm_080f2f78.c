#include "global.h"

extern void func_080F26D8(u32, s8);

void func_080F2F78(u32 *arg0, s32 unused, s8 arg2) {
    (void)unused;
    func_080F26D8(arg0[1], arg2);
}
