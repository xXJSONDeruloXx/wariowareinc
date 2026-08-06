#include "global.h"
#include "graphics.h"

void func_08006EE0(void) {
    register u8 *base asm("r1") = (u8 *)&gGraphicsBuffer;
    register u32 offset asm("r0") = 0x854;
    register u8 *addr asm("r3");
    register u32 field asm("r2");

    addr = base + offset;
    field = *(u16 *)addr;
    offset = 0xF;
    offset &= field;
    *(u16 *)addr = offset;
    offset = 0x858;
    base += offset;
    offset = 0;
    *(u32 *)base = offset;
}
