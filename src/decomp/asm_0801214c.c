#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void dma3_set(const void *, void *, u32, u16, u32);

void func_0801214C(const void *arg0) {
    dma3_set(arg0, (void *)0x03004154, 0x20, 0x20, 0x100);
}
#endif
