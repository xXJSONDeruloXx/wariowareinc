#include "global.h"
#include "src/memory_heap.h"

u32 func_0800557C(void *arg0) {
    u8 *src = (u8 *)arg0;
    register u8 *dst asm("r0");

    dst = (u8 *)mem_heap_alloc(0x10);
    *(u32 *)(dst + 0x0) = *(u32 *)(src + 0x0);
    *(u32 *)(dst + 0x4) = *(u32 *)(src + 0x4);
    *(u32 *)(dst + 0x8) = *(u32 *)(src + 0x8);
    *(u32 *)(dst + 0xC) = *(u32 *)(src + 0xC);
}
