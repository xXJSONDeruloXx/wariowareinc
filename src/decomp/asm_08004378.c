#include "global.h"

extern void mem_heap_dealloc(void *ptr);

struct Func08004378Data {
    u8 padding0[0xC];
    void *fieldC;
    void *field10;
    u8 padding14[4];
    void *field18;
};

void func_08004378(struct Func08004378Data *arg0) {
    mem_heap_dealloc(arg0->fieldC);
    mem_heap_dealloc(arg0->field10);
    if (arg0->field18 != 0) {
        mem_heap_dealloc(arg0->field18);
    }
    mem_heap_dealloc(arg0);
}
