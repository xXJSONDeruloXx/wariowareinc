#include "global.h"

void func_08003D1C(void *arg0) {
    u32 mask = 2;
    u32 val = *(u8 *)arg0;
    mask = -mask;
    mask = val & mask;
    *(u8 *)arg0 = (u8)mask;
}