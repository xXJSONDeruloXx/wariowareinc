#include "global.h"
#include "graphics.h"

void func_0804E290(s32 arg0, s16 arg1) {
    register u8 *base asm("r2");
    register u32 offset asm("r0");
    offset = (u32)(arg0 << 0x10);
    base = (u8 *)&gGraphicsBuffer;
    offset >>= 0x0F;
    base += 0x54;
    offset += (u32)base;
    *(s16 *)offset = arg1;
}
