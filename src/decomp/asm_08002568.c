#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *mem_heap_alloc(u32);
extern void *func_08002124(void *, u32, u32);

void *func_08002568(u32 *arg0) {
    void *result;
    result = mem_heap_alloc(0x5C);
    func_08002124(result, arg0[0], arg0[1]);
    return result;
}
#endif
