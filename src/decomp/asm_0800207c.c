#include "global.h"

extern void func_080F30E0(u32, u32);

void func_0800207C(u32 arg0, u32 arg1) {
    u32 value = arg1 << 16;

    if (arg0 != 0) {
        value >>= 20;
        func_080F30E0(arg0, value);
    }
}
