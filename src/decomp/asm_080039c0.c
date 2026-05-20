#include "global.h"

u32 func_080039C0(u32 *arg0) {
    u8 *p = *(u8 **)arg0 - 2;
    *arg0 = (u32)p;
    return *p | (*(p + 1) << 8);
}
