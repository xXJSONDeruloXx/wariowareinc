#include "global.h"
#include "graphics.h"

void func_0804E290(u32 arg0, u16 arg1) {
    *(u16 *)((u8 *)&gGraphicsBuffer + 0x54 + ((arg0 << 16) >> 15)) = arg1;
}
