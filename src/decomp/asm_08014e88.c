#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"

extern void func_08014E38(void);

typedef void (*Func08014E88SetPalette)(struct SpriteHandler *, s32, s32);

void func_08014E88(s32 arg0) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");

    r4 = r0;
    func_08014E38();
    r0 = (u32)gSpriteHandler;
    r1 = (u32)gCurrentSceneData;
    r2 = 0xCA;
    r2 <<= 1;
    r1 += r2;
    r1 = *(u32 *)r1;
    r4 <<= 1;
    r4 += r1;
    r2 = 2;
    asm volatile("ldrsh %0, [%1, %2]" : "=r"(r1) : "r"(r4), "r"(r2));
    r2 = 0xC;
    ((Func08014E88SetPalette)sprite_set_base_palette)((struct SpriteHandler *)r0, r1, r2);
}
#endif
