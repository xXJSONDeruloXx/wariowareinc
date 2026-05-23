#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/scenes/main_menu.h"

extern void func_0800C7A4(s32);
extern void func_0800C77C(u32);

void func_08013A94(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r4 asm("r4");
    register u32 r5 asm("r5");

    func_0800C7A4(8);
    func_0800C7A4(9);
    r5 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r5;
    r0 += 0xF0;
    r0 = *(u8 *)r0;
    if (r0 <= 0xC) goto skip;
    r4 = (u32)&D_03006518;
    r0 = *(u8 *)(r4 + 3);
    if (r0 <= 1) goto check2;
    func_0800C77C(8);
check2:
    r0 = *(u8 *)(r4 + 3);
    r1 = *(u32 *)r5;
    r1 += 0xE8;
    r1 = *(u8 *)r1;
    r1 -= 2;
    if ((s32)r0 >= (s32)r1) goto skip;
    func_0800C77C(9);
skip:;
}
#endif
