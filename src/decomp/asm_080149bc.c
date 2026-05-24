#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void *gCurrentSceneData;
extern u8 D_083A4A2C;
extern u8 D_083AB35C;

extern void func_0800A240(void *, u32, u32, u32);
extern void func_08005600(void *, u32, void *, s16 *);

void func_080149BC(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");

    r0 = (u32)&D_083A4A2C;
    r4 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r4;
    r2 = 0xA0;
    r2 <<= 1;
    r1 = r1 + r2;
    r1 = *(u32 *)r1;
    r2 = 0;
    r3 = 0;
    func_0800A240((void *)r0, r1, r2, r3);

    r0 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r0;
    r1 = *(u32 *)r4;
    r1 = *(u32 *)(r1 + 4);
    r2 = (u32)&D_083AB35C;
    r3 = (u32)&gCurrentSceneSpritePool;
    r3 = *(u32 *)r3;
    func_08005600((void *)r0, r1, (void *)r2, (s16 *)r3);

    r1 = *(u32 *)r4;
    r1 += 0xDE;
    r0 = *(u8 *)r1;
    r2 = 0x10;
    r0 = r0 | r2;
    *(u8 *)r1 = r0;
}
#endif
