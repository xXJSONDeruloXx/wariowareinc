#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern u32 get_current_language(void);
extern u32 D_083AB320[];

typedef void (*Func08014374Load)(u32);

void func_08014374(void) {
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");

    r0 = get_current_language();
    r1 = (u32)D_083AB320;
    r0 <<= 2;
    r0 += r1;
    r1 = *(u32 *)&gCurrentSceneData;
    r1 += 0xFD;
    r1 = *(u8 *)r1;
    r0 = *(u32 *)r0;
    r1 <<= 2;
    r1 += r0;
    r0 = *(u32 *)r1;
    ((Func08014374Load)func_08015A88)(r0);
}
#endif
