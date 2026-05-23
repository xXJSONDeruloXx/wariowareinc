#if __INCLUDE_LEVEL__ > 0
#include "global.h"

/* sprite_is_invalid, D_03000E70: from lib_sprite.h */
struct Sprite;
struct SpriteHandler;

s32 func_080EFC20(struct SpriteHandler *arg0) {
    register s32 count asm("r2");
    register struct Sprite *data asm("r3");
    register s32 animIdx asm("r1");
    register s32 sentinel asm("r4");
    register s32 temp asm("r0");

    count = 0;
    data = *(struct Sprite **)((u8 *)arg0 + 8);
    sentinel = 0xC;
    animIdx = *(s16 *)((u8 *)arg0 + sentinel);
    temp = 1;
    temp = -temp;
    if (animIdx == temp) goto done;
    sentinel = temp;
    loop:
    count++;
    temp = animIdx << 3;
    temp -= animIdx;
    temp <<= 3;
    temp += (s32)data;
    animIdx = *(s16 *)((u8 *)temp + 0x1A);
    if (animIdx != sentinel) goto loop;
    done:
    return count;
}

__attribute__((section(".text"))) const u8 _padding_func_080EFC20[] = { 0x00, 0x00 };
#endif
