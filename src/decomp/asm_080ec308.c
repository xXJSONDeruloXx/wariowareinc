#include "global.h"
#include "types.h"

void func_080EC308(void) {
    register u8 *r2 asm("r2") = (u8 *)&gCurrentSceneVariable;
    register u8 *r0 asm("r0") = *(u8 **)r2;
    register u32 r1 asm("r1");
    r0 += 0xF8;
    r1 = 1;
    *r0 = r1;
    r0 = *(u8 **)r2;
    r0 += 0xF9;
    r1 = 0x14;
    *r0 = r1;
}
