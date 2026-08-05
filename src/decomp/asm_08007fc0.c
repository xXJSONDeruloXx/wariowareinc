#include "global.h"
#include "src/memory_heap.h"

u32 func_08007FC0(u32 arg0) {
    register u8 *base asm("r0");
    register u8 *tail asm("r2");

    base = (u8 *)mem_heap_alloc(0x2C);
    *(u32 *)base = arg0;
    tail = base;
    tail += 0x28;
    *(u8 *)tail = 0;
}
