#include "global.h"

void mem_heap_dealloc(u32);

void func_08005FA0(u32 *arg0) {
    mem_heap_dealloc(arg0[0]);
    mem_heap_dealloc((u32)arg0);
}
