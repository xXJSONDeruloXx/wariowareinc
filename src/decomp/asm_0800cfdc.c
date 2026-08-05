#include "global.h"
#include "src/code_08000f10.h"

extern u8 D_030043D4[];

void func_0800CFDC(const void *arg0) {
    dma3_set(arg0, D_030043D4, 0x80, 0x10, 0x100);
}
