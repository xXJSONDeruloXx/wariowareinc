#include "global.h"

void func_080CD564(u32 *arg0, u32 *arg1) {
    register u32 *dst asm("r3") = arg0;
    register u32 value1 asm("r2");
    register u32 value2 asm("r1");

    value1 = arg1[10];
    dst[10] = value1;
    value2 = arg1[11];
    dst[11] = value2;
}
