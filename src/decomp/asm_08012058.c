#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/scenes/main_menu.h"

extern void func_08011730(u32);
extern u8 D_083AA0C4[];
extern void func_08011920(void);

typedef void (*Func08012058Schedule)(s32, s32, u32, u32);

void func_08012058(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r3 asm("r3");

    r0 = (u32)&D_03006518;
    r0 = *(u8 *)r0;
    r0 <<= 4;
    r1 = (u32)D_083AA0C4;
    r0 += r1;
    r1 = *(u32 *)(r0 + 0xC);
    r2 = 0;
    r0 = *(s16 *)(r1 + r2);
    r2 = 2;
    r1 = *(s16 *)(r1 + r2);
    r2 = (u32)func_08011920 + 1;
    r3 = 0;
    ((Func08012058Schedule)func_08011504)(r0, r1, r2, r3);
    func_08011730(0);
}
#endif
