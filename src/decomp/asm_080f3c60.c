#include "global.h"

extern u8 D_030068F0[];

void func_080F3C60(u8 arg0, u8 arg1, u8 arg2, u8 arg3) {
    D_030068F0[0] = arg0;
    D_030068F0[1] = arg1;
    D_030068F0[2] = arg2;
    D_030068F0[3] = arg3;
}
