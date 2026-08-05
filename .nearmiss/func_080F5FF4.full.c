#include "global.h"

extern void func_080F56EC(u32 *, u32, u32, u32, u32);

void func_080F5FF4(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    u32 args[4];

    args[0] = arg0;
    args[1] = arg1;
    args[2] = arg2;
    args[3] = arg3;
    func_080F56EC(args, arg0, arg1, arg2, arg3);
}
