#include "global.h"

void func_080F2774(void *arg0) {
    u32 zero = 0;
    u8 *p = (u8 *)arg0;
    p[6] = 1;
    *(u32 *)(p + 8) = zero;
    p[7] = (u8)zero;
}