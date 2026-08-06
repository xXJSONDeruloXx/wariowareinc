#include "global.h"

extern u32 *D_083C8B64;

void func_08024E34(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    u32 *base = D_083C8B64;

    base[8] = arg0;
    base[9] = arg1;
    base[10] = arg2;
    base[11] = arg3;
}
