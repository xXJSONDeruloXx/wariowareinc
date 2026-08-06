#include "global.h"
#include "graphics.h"

void func_0801F698(void) {
    register u8 *base asm("r1") = (u8 *)&gGraphicsBuffer;
    register u32 field asm("r2") = *(u16 *)base;
    register u32 value asm("r3");
    register u32 result asm("r0");

    value = 0x80;
    value <<= 8;
    result = value;
    result |= field;
    *(u16 *)base = result;
    base += 0x46;
    result = 0xFC;
    result <<= 6;
    *(u16 *)base = result;
}
