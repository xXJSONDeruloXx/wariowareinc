#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void func_0800C7A4(s32);
typedef void (*Func08011774SetCel)(struct SpriteHandler *, s32, s32);

void func_08011774(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");

    r4 = 0;
loop:
    r0 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r0;
    r1 = (u32)&gCurrentSceneSpritePool;
    r2 = *(u32 *)r1;
    r1 = r4 << 1;
    r1 += r2;
    r2 = 2;
    r1 = *(s16 *)(r1 + r2);
    r2 = 1;
    ((Func08011774SetCel)sprite_set_anim_cel)((struct SpriteHandler *)r0, r1, r2);
    r4 += 1;
    if (r4 <= 2) goto loop;
    r0 = 0xA;
    func_0800C7A4(r0);
}
#endif
