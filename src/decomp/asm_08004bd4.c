#include "global.h"
#include "src/memory_heap.h"

struct Func08004BD4Record {
    u32 value0;
    u32 value4;
    u32 value8;
    u32 valueC;
};

u32 func_08004BD4(void *arg0) {
    struct Func08004BD4Record *src;
    struct Func08004BD4Record *dst;

    src = (struct Func08004BD4Record *)arg0;
    dst = (struct Func08004BD4Record *)mem_heap_alloc(0x10);
    dst->value0 = src->value0;
    dst->value4 = src->value4;
    dst->value8 = src->value8;
    dst->valueC = 0;
    return (u32)dst;
}
