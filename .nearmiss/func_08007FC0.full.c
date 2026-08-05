#include "global.h"
#include "src/memory_heap.h"

void func_08007FC0(u32 arg0) {
    u8 *base = (u8 *)mem_heap_alloc(0x2C);

    *(u32 *)base = arg0;
    *(u8 *)(base + 0x28) = 0;
}
