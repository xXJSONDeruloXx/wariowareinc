#include "global.h"
#include "src/memory_heap.h"

struct Func0800557CRecord {
    u32 value0;
    u32 value4;
    u32 value8;
    u32 valueC;
};

u32 func_0800557C(void *arg0) {
    struct Func0800557CRecord *src;
    struct Func0800557CRecord *dst;

    src = (struct Func0800557CRecord *)arg0;
    dst = (struct Func0800557CRecord *)mem_heap_alloc(0x10);
    dst->value0 = src->value0;
    dst->value4 = src->value4;
    dst->value8 = src->value8;
    dst->valueC = src->valueC;
    return (u32)dst;
}
