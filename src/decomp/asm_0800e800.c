#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"

extern void sprite_set_x_y(void *, s32, s32, s32);

void func_0800E800(u32 arg0, u32 arg1) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2") = arg0;
    register u32 r3 asm("r3") = arg1;
    register u32 r4 asm("r4");

    r0 = (u32)&gSpriteHandler;
    r0 = *(u32 *)r0;
    r1 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r1;
    r4 = 0xB4;
    r4 <<= 2;
    r1 += r4;
    r4 = 0;
    r1 = *(s16 *)(r1 + r4);
    r2 <<= 16;
    r2 = (u32)((s32)r2 >> 16);
    r3 <<= 16;
    r3 = (u32)((s32)r3 >> 16);
    sprite_set_x_y((void *)r0, (s32)r1, (s32)r2, (s32)r3);
}
