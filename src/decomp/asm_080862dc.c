#include "global.h"
#include "graphics.h"

void func_080862DC(u16 arg0) {
    gGraphicsBuffer.unk4C = 0xC4;
    *(u16*)((u8*)&gGraphicsBuffer + 0x50) = arg0;
}
