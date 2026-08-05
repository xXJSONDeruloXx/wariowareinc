#include "global.h"
#include "types.h"

void func_080DF440(void) {
    register u32 r1 asm("r1") = (u32)&gCurrentSceneVariable;
    register u8 *r0 asm("r0");
    register u32 r2 asm("r2");
    r0 = *(u8 **)r1;
    r2 = 0;
    r0[0xF] = r2;
    r0 = *(u8 **)r1;
    r1 = 0;
    *(u16 *)(r0 + 0xC) = r2;
    r0[0xE] = r1;
}
