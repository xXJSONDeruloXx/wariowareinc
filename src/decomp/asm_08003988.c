#include "global.h"

void func_08003988(void **a0, s32 a1) {
    u8 *p;
    p = (u8 *)*a0;
    *p = (u8)a1;
    p++;
    *p = (u8)((u32)a1 >> 8);
    p++;
    *a0 = p;
}