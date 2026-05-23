#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "types.h"

extern u32 D_083AB456[];
extern u32 D_030041E4[];

void func_080159FC(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r2 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r2;
    r1 += 0xCC;
    r0 = *(u8 *)r1;
    r0 += 1;
    *(u8 *)r1 = r0;
    r0 <<= 24;
    r0 >>= 24;
    if (r0 <= 0xB) goto skip_reset;
    r0 = *(u32 *)r2;
    r0 += 0xCC;
    r1 = 0;
    *(u8 *)r0 = r1;
skip_reset:
    r0 = *(u32 *)r2;
    r0 += 0xCC;
    r0 = *(u8 *)r0;
    r0 >>= 2;
    r3 = 0;
    r1 = (u32)D_083AB456;
    r0 <<= 3;
    r1 = r0 + r1;
    r2 = (u32)D_030041E4;
loop:
    r0 = *(u16 *)r1;
    *(u16 *)r2 = r0;
    r1 += 2;
    r2 += 2;
    r3 += 1;
    if (r3 <= 3) goto loop;
}
#endif
