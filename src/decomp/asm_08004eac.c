#include "global.h"

extern void mem_heap_dealloc(void *);

void func_08004EAC(void *arg0) {
    u8 *base = (u8 *)arg0;

    mem_heap_dealloc(*(void **)(base + 8));
    mem_heap_dealloc(*(void **)(base + 0xC));
    mem_heap_dealloc(arg0);
}
