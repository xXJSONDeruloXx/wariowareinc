#include "global.h"
#include "graphics.h"

void func_0800CDB0(u32);

void func_0809CE64(void) {
    gGraphicsBuffer.unk4C = 0;
    *(u16 *)((u8 *)&gGraphicsBuffer + 0x4E) = 0;
    func_0800CDB0(1);
}
