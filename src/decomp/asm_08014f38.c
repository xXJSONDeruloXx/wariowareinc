#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void *gCurrentSceneData;
extern void scene_set_current_thread(u32);
extern void func_08014E88(s32);

typedef void (*Func08014F38CallE88)(s32);
typedef void (*Func08014F38SetVisible)(struct SpriteHandler *, s32, s32);

void func_08014F38(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r0 = 0;
    scene_set_current_thread(r0);

    r5 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r5;
    r1 = 0xC2;
    r1 <<= 1;
    r0 = r0 + r1;
    r2 = 0;
    r0 = *(s16 *)(r0 + r2);
    ((Func08014F38CallE88)func_08014E88)((s32)r0);

    r4 = 0;
    goto loop_check;

loop_body:
    r0 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r0;
    r1 = *(u32 *)r5;
    r2 = 0xCA;
    r2 <<= 1;
    r1 = r1 + r2;
    r2 = *(u32 *)r1;
    r1 = r4 << 1;
    r1 = r1 + r2;
    r2 = 0;
    r1 = *(s16 *)(r1 + r2);
    r2 = 1;
    ((Func08014F38SetVisible)sprite_set_visible)((struct SpriteHandler *)r0, r1, r2);
    r4 += 1;

loop_check:
    r0 = *(u32 *)r5;
    r1 = 0xC8;
    r1 <<= 1;
    r0 = r0 + r1;
    r0 = *(u8 *)r0;
    r0 += 1;
    if ((u32)r4 < r0) goto loop_body;

    r0 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r0;
    r1 += 0xDE;
    r2 = *(u8 *)r1;
    r0 = 0x41;
    r0 = -r0;
    r0 = r2 & r0;
    *(u8 *)r1 = r0;
}
#endif
