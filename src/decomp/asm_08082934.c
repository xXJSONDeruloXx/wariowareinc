#include "global.h"
#include "graphics.h"

void func_08082934(u32 arg0) {
    register u32 offset asm("r0") = arg0;
    register u8 *base asm("r1");
    u16 value;
    offset <<= 0x18;
    base = (u8 *)&gGraphicsBuffer;
    offset >>= 0x17;
    offset = (u32)base + offset;
    offset += 0x9A;
    value = *(u16 *)offset;
    base += 0x7A;
    *(u16 *)base = value;
}
