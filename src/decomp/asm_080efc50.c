#if __INCLUDE_LEVEL__ > 0
#include "global.h"

struct Sprite;
struct SpriteHandler;

s32 func_080EFC50(struct SpriteHandler *arg0, u32 arg1) {
    register u32 r4 asm("r4");
    register s32 count asm("r3");
    register struct Sprite *data asm("r1");
    register s32 animIdx asm("r2");
    register s32 sentinel asm("r0");
    register s32 sentinelSave asm("r5");
    register s32 computed asm("r0");
    register struct Sprite *entry asm("r2");
    register s32 unk30 asm("r0");
    register s32 nextAnim asm("r2");
    register s32 offset asm("r0");

    r4 = arg1;
    count = 0;
    data = *(struct Sprite **)((u8 *)arg0 + 8);
    sentinel = 0xC;
    animIdx = *(s16 *)((u8 *)arg0 + sentinel);
    sentinel = 1;
    sentinel = -sentinel;
    if (animIdx == sentinel) goto done;
    sentinelSave = sentinel;
    loop:
    computed = animIdx << 3;
    computed -= animIdx;
    computed <<= 3;
    computed += (s32)data;
    entry = (struct Sprite *)computed;
    unk30 = *(u32 *)((u8 *)entry + 0x30);
    if (unk30 == r4) count++;
    offset = 0x1A;
    nextAnim = *(s16 *)((u8 *)entry + offset);
    if (nextAnim != sentinelSave) goto loop;
    done:
    return count;
}

__attribute__((section(".text"))) const u8 _padding_func_080EFC50[] = { 0x00, 0x00 };
#endif
