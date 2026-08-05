#include "global.h"

extern void func_080F30E0(u32, u32);

void func_0800207C(u32 arg0, u32 arg1) {
    if (arg0 != 0) {
        func_080F30E0(arg0, (u32)(arg1 << 0x10) >> 0x14);
    }
}
