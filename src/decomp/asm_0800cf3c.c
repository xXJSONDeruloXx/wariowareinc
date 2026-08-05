#include "global.h"
#include "src/code_08000f10.h"

extern u8 D_03004054[];

void func_0800CF3C(const void *arg0) {
    dma3_set(arg0, D_03004054, 0x200, 0x10, 0x100);
}
