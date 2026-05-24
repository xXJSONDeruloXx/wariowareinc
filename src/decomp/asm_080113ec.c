#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"
#include "scenes.h"

extern void func_080122FC(void);
extern void func_08013188(void);

void func_080113EC(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 r2 asm("r2");
    register u32 r4 asm("r4");

    r4 = (u32)&gCurrentSceneData;
    r0 = *(u32 *)r4;
    r0 += 0xDD;
    r1 = *(u8 *)r0;
    r0 = r1 << 30;
    if ((s32)r0 < 0) goto done;
    r0 = r1 << 28;
    if ((s32)r0 < 0) goto done;
    r0 = r1 << 29;
    if ((s32)r0 >= 0) goto skip_first;
    func_080122FC();
    r0 = *(u32 *)r4;
    r0 += 0xDD;
    r2 = *(u8 *)r0;
    r1 = 5;
    r1 = -r1;
    r1 &= r2;
    *(u8 *)r0 = r1;
skip_first:
    r0 = *(u32 *)r4;
    r0 += 0xDD;
    r0 = *(u8 *)r0;
    r0 <<= 27;
    if ((s32)r0 >= 0) goto done;
    func_08013188();
    r0 = *(u32 *)r4;
    r0 += 0xDD;
    r2 = *(u8 *)r0;
    r1 = 0x11;
    r1 = -r1;
    r1 &= r2;
    *(u8 *)r0 = r1;
done:;
}
#endif
