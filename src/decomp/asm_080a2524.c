#include "global.h"
#include "graphics.h"

void func_080A2524(void) {
    gGraphicsBuffer.unk4C = 0;
    *(u16 *)((u8 *)&gGraphicsBuffer + 0x4E) = 0;
}
