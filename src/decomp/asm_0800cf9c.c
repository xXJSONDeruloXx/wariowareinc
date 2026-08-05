#include "global.h"
#include "src/code_08000f10.h"

extern u8 D_03004254[];

void func_0800CF9C(const void *arg0) {
    dma3_set(arg0, D_03004254, 0x140, 0x10, 0x100);
}
