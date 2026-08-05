#include "global.h"
#include "src/memory_heap.h"

u32 func_08004BD4(void *arg0) {
    u8 *src = (u8 *)arg0;
    u8 *dst = (u8 *)mem_heap_alloc(0x10);

    *(u32 *)(dst + 0x0) = *(u32 *)(src + 0x0);
    *(u32 *)(dst + 0x4) = *(u32 *)(src + 0x4);
    *(u32 *)(dst + 0x8) = *(u32 *)(src + 0x8);
    *(u32 *)(dst + 0xC) = 0;
}
