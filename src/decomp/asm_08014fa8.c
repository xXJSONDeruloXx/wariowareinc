#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern void scene_set_current_thread(u32);
extern void func_080065C0(u32);
extern void mem_heap_dealloc(u32);

void func_08014FA8(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");

    scene_set_current_thread(0);
    r4 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r4;
    r1 = 0xBE;
    r1 <<= 1;
    r0 += r1;
    r0 = *(u32 *)r0;
    func_080065C0(r0);
    r0 = *(u32 *)r4;
    r1 = 0xD0;
    r1 <<= 1;
    r0 += r1;
    r0 = *(u32 *)r0;
    mem_heap_dealloc(r0);
    r1 = *(u32 *)r4;
    r1 += 0xDE;
    r2 = *(u8 *)r1;
    r0 = 0x41;
    r0 = -r0;
    r0 &= r2;
    *(u8 *)r1 = r0;
    r0 = *(u32 *)r4;
    r1 = 0xC0;
    r1 <<= 1;
    r0 += r1;
    r0 = *(u32 *)r0;
    ((void (*)(void))r0)();
}
#endif
