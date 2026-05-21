#include "global.h"

void func_08001B28(s32);
void mem_heap_dealloc(u32);

void func_0800C764(s16 *arg0) {
    func_08001B28(*arg0);
    mem_heap_dealloc((u32)arg0);
}
