#include "global.h"
#include "graphics.h"

void func_08082934(u32 arg0) {
    u8 *base = (u8 *)&gGraphicsBuffer;
    u32 offset = (arg0 << 0x18) >> 0x17;
    *(u16 *)(base + 0x7A) = *(u16 *)(base + offset + 0x9A);
}
