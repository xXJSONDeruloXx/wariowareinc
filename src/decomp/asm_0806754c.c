#include "global.h"

void func_0806754C(s32 arg0, s32 arg1, s32 *arg2) {
    arg0 >>= 8;
    arg1 >>= 8;
    arg2[0] = arg0 - 2;
    arg2[2] = arg0 + 2;
    arg2[1] = arg1 - 2;
    arg2[3] = arg1 + 2;
}
