#include "global.h"

void func_080351A4(u32 a0, u8 *a1) {
    a0 = (u32)(s16)a0;
    a1 += 0x80;
    a1 += a0;
    *a1 = 3;
}
