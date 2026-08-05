#include "global.h"
#include "src/code_08000f10.h"

extern u8 D_03004454[];

void func_08006B68(void) {
    dma3_fill(0x22222222, D_03004454, 0x400, 0x20, 0x100);
}
