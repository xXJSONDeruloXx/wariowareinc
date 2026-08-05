#include "global.h"

u32 func_08035ACC(const u8 *arg0) {
    u32 r2 = *(u32 *)(arg0 + 0x70);
    u32 r1 = *(u32 *)(arg0 + 0x7C);
    u32 r0 = r2 - r1;
    if (r2 < r1)
        r0 = r1 - r2;
    return r0;
}
