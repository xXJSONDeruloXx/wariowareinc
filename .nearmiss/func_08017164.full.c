#include "global.h"
#include "src/code_08000f10.h"

extern void func_08003DF4(void *, void *);

void func_08017164(u32 arg0) {
    void *source;
    void *destination;

    source = (void *)(arg0 & 0x7FFFFFFF);
    destination = (void *)(VRAMBase + 0x8000);
    if ((arg0 & 0x80000000) != 0) {
        dma3_set(source, destination, 0x5000, 0x20, 0x100);
    } else {
        func_08003DF4(source, destination);
    }
}
