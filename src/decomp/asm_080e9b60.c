#include "global.h"
#include "graphics.h"

void func_0800418C(void);

void func_080E9B60(void) {
    gGraphicsBuffer.unk4C = 0;
    *(u16 *)((u8 *)&gGraphicsBuffer + 0x4E) = 0;
    func_0800418C();
}
