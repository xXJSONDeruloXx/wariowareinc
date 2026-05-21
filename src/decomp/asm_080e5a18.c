#include "global.h"

void func_080E5A18(s32 *a0, s32 a1, s32 a2, s32 a3) {
    a0[1] = a1;
    a0[2] = a2;
    a0[3] = a3;
    a0[5] = 0;
    a0[4] = 0;
    ((u8 *)a0)[0x18] = 0;
}