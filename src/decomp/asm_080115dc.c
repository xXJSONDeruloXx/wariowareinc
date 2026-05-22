#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/code_08000f10.h"

extern void *gCurrentSceneData;

void func_080115DC(void) {
    register u8 *data asm("r1");
    register u8 *check asm("r0");
    register const void *src asm("r0");
    register void *dest asm("r1");
    register u32 size asm("r2");
    register u32 unit asm("r3");

    data = gCurrentSceneData;
    check = data;
    check += 0xDC;
    if (*check != 0) {
        src = *(void **)(data + 0xD4);
        dest = *(void **)(data + 0xD8);
        size = 0xA0 << 3;
        unit = 0x80 << 1;
        dma3_set(src, dest, size, 0x20, unit);
    }
}
#endif
