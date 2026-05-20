#include "global.h"

void func_080F0E30(u32 arg0, u32 arg1) {
    u8 *p = *(u8 **)0x030068E8;
    p[(arg0 << 5) + 1] = arg1;
}
