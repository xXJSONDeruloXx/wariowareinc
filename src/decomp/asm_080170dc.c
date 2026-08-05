#include "global.h"
#include "src/code_08000f10.h"

extern u8 D_030041D4[];

void func_080170DC(const void *arg0) {
    dma3_set(arg0, D_030041D4, 0x80, 0x20, 0x100);
}
