#include "global.h"
#include "src/code_08000f10.h"

void func_08004F14(void *arg0, void *arg1) {
    dma3_set(arg0, arg1, 0x40, 0x10, 0x100);
}
