#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void *gCurrentSceneData;
extern u8 D_083A4A2C;
extern void func_0800A240(void *, u32, u32, u32);
extern void func_08005600(void *, u32, void *, s16 *);
extern void mem_heap_dealloc(u32);

void func_0801522C(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");
    register u32 r6 asm("r6");

    r0 = (u32)&D_083A4A2C;
    r4 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r4;
    r2 = 0xBE;
    r2 <<= 1;
    r1 = r1 + r2;
    r1 = *(u32 *)r1;
    r2 = 0;
    r3 = 0;
    func_0800A240((void *)r0, r1, r2, r3);

    r0 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r0;
    r3 = *(u32 *)r4;
    r1 = *(u32 *)(r3 + 4);
    r6 = 0xCE;
    r6 <<= 1;
    r2 = r3 + r6;
    r2 = *(u32 *)r2;
    r5 = 0xCA;
    r5 <<= 1;
    r3 = r3 + r5;
    r3 = *(u32 *)r3;
    func_08005600((void *)r0, r1, (void *)r2, (s16 *)r3);

    r0 = *(u32 *)r4;
    r0 = r0 + r5;
    r0 = *(u32 *)r0;
    mem_heap_dealloc(r0);

    r0 = *(u32 *)r4;
    r1 = 0xCC;
    r1 <<= 1;
    r0 = r0 + r1;
    r0 = *(u32 *)r0;
    mem_heap_dealloc(r0);

    r0 = *(u32 *)r4;
    r0 = r0 + r6;
    r0 = *(u32 *)r0;
    mem_heap_dealloc(r0);

    r1 = *(u32 *)r4;
    r1 += 0xDE;
    r0 = *(u8 *)r1;
    r2 = 0x40;
    r0 = r0 | r2;
    *(u8 *)r1 = r0;
}
#endif
