#include "global.h"

void func_080029D0(void *a0) {
    register u8 v asm("r2");
    register u8 m asm("r1");
    v = *(u8 *)a0;
    m = 3;
    m = -m;
    m = v & m;
    *(u8 *)a0 = m;
    *(u16 *)a0 = *(u16 *)a0 & 3;
}
