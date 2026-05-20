#include "memory_heap.h"

asm(".include \"include/gba.inc\"");

void *func_08006184(u16 heapId, u32 size);

void *mem_heap_alloc(u32 size) {
    return func_08006184(0, size);
}

#include "asm/memory_heap/asm_08006184.s"

#include "asm/memory_heap/asm_08006240.s"
