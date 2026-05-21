#include "global.h"

extern void func_080F3100(u32, u32);

void func_08002068(u32 arg0, u32 arg1) {
    arg1 = arg1 << 16;
    if (arg0 != 0) {
        arg1 = arg1 >> 20;
        func_080F3100(arg0, arg1);
    }
}
