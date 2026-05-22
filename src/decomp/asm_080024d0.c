#if __INCLUDE_LEVEL__ > 0
#include "global.h"

void func_080024D0(u32 *a0, u32 a1, u32 a2, u32 a3) {
    a0[0] = a1;
    a0[1] = a2;
    a0[2] = a3;
    a0 = a0 + 3;
    a0[1] = 0;
    a0[0] = 0;
    a0[2] = 0;
}
#endif
