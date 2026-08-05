#include "global.h"
#include "src/memory_heap.h"

void func_0801E6F8(void) {
    register u8 **anchor asm("r4") = (u8 **)&gCurrentSceneVariable;
    u8 *base = *anchor;

    mem_heap_dealloc(*(void **)(base + 0xB4));
    base = *anchor;
    base[0x10] &= (u8)-3;
}
