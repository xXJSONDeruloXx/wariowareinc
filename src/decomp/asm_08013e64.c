#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"

extern u16 gPressedKeys;
extern void *gCurrentSceneData;
extern void func_08011584(void);
extern void func_08013A4C(void);
extern u32 func_08011698(void);
extern void func_080137B0(void);
extern void func_080139D4(void);

void func_08013E64(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r4 asm("r4");

    r0 = (u32)&gPressedKeys;
    r1 = *(u16 *)r0;
    r0 = 0xF1;
    r0 = r1 & r0;
    if (r0 == 0) goto call_11698;

    r4 = (u32)&gCurrentSceneData;
    r1 = *(u32 *)r4;
    r0 = r1;
    r0 += 0xF1;
    r0 = *(u8 *)r0;
    if (r0 == 0) goto call_11698;

    r0 = r1;
    r0 += 0xDD;
    r0 = *(u8 *)r0;
    r0 <<= 0x1F;
    if (r0 == 0) goto check2;
    func_08011584();

check2:
    r0 = *(u32 *)r4;
    r0 += 0xDD;
    r0 = *(u8 *)r0;
    r0 <<= 0x19;
    if ((s32)r0 >= 0) goto call_11698;
    func_08013A4C();

call_11698:
    r0 = func_08011698();
    if (r0 == 0) goto call_139d4;
    func_080137B0();

call_139d4:
    func_080139D4();
}
#endif
