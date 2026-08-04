#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern u8 D_03000E70;
extern s32 sprite_is_invalid(void *, s16);
extern void sprite_remove_z_link(void *, s16);
extern void sprite_handler_dealloc_id(void *, s16);

typedef s32 (*FuncSpriteInvalid)(void *, s32);
typedef void (*FuncSpriteLink)(void *, s32);

void sprite_delete(void *arg0, s32 arg1) {
    register u32 r0 asm("r0") = (u32)arg0;
    register u32 r1 asm("r1") = (u32)arg1;
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    r5 = r0;
    asm volatile("" : "+r"(r0), "+r"(r1), "+r"(r5));
    r2 = (u32)&D_03000E70;
    r0 = 4;
    *(u8 *)r2 = r0;
    r1 <<= 16;
    r4 = (u32)((s32)r1 >> 16);
    r0 = r5;
    r1 = r4;
    r0 = ((FuncSpriteInvalid)sprite_is_invalid)((void *)r0, r1);
    if (r0 != 0) goto done;
    r2 = *(u32 *)(r5 + 8);
    r1 = r4 << 3;
    r1 -= r4;
    r1 <<= 3;
    r2 = r1 + r2;
    r3 = *(u8 *)r2;
    r0 = 2;
    r0 = -r0;
    r0 &= r3;
    *(u8 *)r2 = r0;
    r0 = *(u32 *)(r5 + 8);
    r1 += r0;
    r2 = *(u8 *)(r1 + 1);
    r0 = 0x41;
    r0 = -r0;
    r0 &= r2;
    *(u8 *)(r1 + 1) = r0;
    r0 = r5;
    r1 = r4;
    ((FuncSpriteLink)sprite_remove_z_link)((void *)r0, r1);
    r0 = r5;
    r1 = r4;
    ((FuncSpriteLink)sprite_handler_dealloc_id)((void *)r0, r1);
done:;
}
#endif
