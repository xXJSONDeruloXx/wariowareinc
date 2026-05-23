#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/scenes/main_menu.h"

extern void func_08013C60(void);
extern void func_08013AF4(void);

void func_08013A4C(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");

    r2 = (u32)&gGraphicsBuffer;
    r4 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r4;
    r1 = r0;
    r1 += 0xEC;
    r1 = *(u16 *)r1;
    *(u16 *)(r2 + 0x14) = r1;
    r0 += 0xDD;
    r0 = *(u8 *)r0;
    r0 <<= 0x1F;
    if (r0 != 0) goto skip;
    func_08013C60();
    func_08013AF4();
    r0 = *(u32 *)r4;
    r0 += 0xF1;
    r1 = 0;
    *(u8 *)r0 = r1;
skip:
    r0 = *(u32 *)r4;
    r0 += 0xDD;
    r2 = *(u8 *)r0;
    r1 = 0x41;
    r1 = -r1;
    r1 &= r2;
    *(u8 *)r0 = r1;
}
#endif
