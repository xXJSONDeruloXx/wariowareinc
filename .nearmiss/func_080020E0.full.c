#include "global.h"

extern void func_080F2F78(u32, s32, s8);

void func_080020E0(u32 arg0, u8 arg1) {
    if (arg0 != 0) {
        func_080F2F78(arg0, -1, (s8)arg1);
    }
}
